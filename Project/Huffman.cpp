#include "Huffman.h"
#include <stdexcept>
#include <cstring>

namespace {
    struct Cmp {
        bool operator()(const std::unique_ptr<Huffman::Node>& a,
                        const std::unique_ptr<Huffman::Node>& b) const {
            return a->freq > b->freq; // min-heap
        }
    };
}

void Huffman::writeU32(std::ostream& out, uint32_t v) {
    for (int i=0;i<4;++i) out.put(static_cast<char>((v >> (8*i)) & 0xFF));
}
void Huffman::writeU64(std::ostream& out, uint64_t v) {
    for (int i=0;i<8;++i) out.put(static_cast<char>((v >> (8*i)) & 0xFF));
}
uint32_t Huffman::readU32(std::istream& in) {
    uint32_t v=0; for (int i=0;i<4;++i){ int c=in.get(); if(c==EOF) throw std::runtime_error("Unexpected EOF"); v |= (uint32_t(uint8_t(c)) << (8*i)); }
    return v;
}
uint64_t Huffman::readU64(std::istream& in) {
    uint64_t v=0; for (int i=0;i<8;++i){ int c=in.get(); if(c==EOF) throw std::runtime_error("Unexpected EOF"); v |= (uint64_t(uint8_t(c)) << (8*i)); }
    return v;
}

std::unique_ptr<Huffman::Node> Huffman::buildTree(const std::array<uint64_t,256>& freq) {
    std::priority_queue<std::unique_ptr<Node>, std::vector<std::unique_ptr<Node>>, Cmp> pq;
    for (int i=0;i<256;++i) {
        if (freq[i] > 0) {
            pq.push(std::make_unique<Node>(freq[i], int16_t(i)));
        }
    }
    if (pq.empty()) {
        // empty input; create a dummy node
        return std::make_unique<Node>(1, int16_t(0));
    }
    if (pq.size() == 1) {
        // Edge case: only one symbol. Make a tree with a single child.
        auto lone = std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
        pq.pop();
        auto parent = std::make_unique<Node>(lone->freq, std::move(lone), std::make_unique<Node>(0, int16_t((lone->symbol+1)%256)));
        return parent;
    }
    while (pq.size() > 1) {
        auto a = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
        auto b = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
        auto parent = std::make_unique<Node>(a->freq + b->freq, std::move(a), std::move(b));
        pq.push(std::move(parent));
    }
    auto root = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
    return root;
}

void Huffman::buildCodes(const Node* node, std::vector<bool>& path, CodeMap& codes) {
    if (!node) return;
    if (node->symbol >= 0) {
        if (path.empty()) { path.push_back(false); } // single-symbol edge case
        codes[static_cast<uint8_t>(node->symbol)] = path;
        return;
    }
    path.push_back(false);
    buildCodes(node->left.get(), path, codes);
    path.back() = true;
    buildCodes(node->right.get(), path, codes);
    path.pop_back();
}

void Huffman::compress(std::istream& in, std::ostream& out) {
    // Read input and collect bytes
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    uint64_t orig_size = data.size();

    std::array<uint64_t,256> freq{};
    for (auto b : data) freq[b]++;

    auto root = buildTree(freq);

    CodeMap codes{};
    std::vector<bool> path;
    buildCodes(root.get(), path, codes);

    // Header
    out.write("CMP1", 4);
    out.put(char(0x01)); // algo id: Huffman
    writeU64(out, orig_size);
    for (int i=0;i<256;++i) writeU32(out, static_cast<uint32_t>(freq[i]));

    // Bit-pack
    uint8_t buffer = 0; int bitcount = 0;
    auto flush_bit = [&](bool bit){
        buffer |= (uint8_t(bit) << bitcount);
        bitcount++;
        if (bitcount == 8) {
            out.put(static_cast<char>(buffer));
            buffer = 0; bitcount = 0;
        }
    };

    for (auto b : data) {
        const auto& code = codes[b];
        for (bool bit : code) flush_bit(bit);
    }
    // pad remaining bits
    if (bitcount > 0) {
        out.put(static_cast<char>(buffer));
    }
}

void Huffman::decompress(std::istream& in, std::ostream& out) {
    char magic[4];
    if (!in.read(magic, 4)) throw std::runtime_error("Bad header");
    if (std::memcmp(magic, "CMP1", 4) != 0) throw std::runtime_error("Not CMP1");
    int algo = in.get();
    if (algo != 0x01) throw std::runtime_error("Wrong algo id for Huffman");
    uint64_t orig_size = readU64(in);
    std::array<uint64_t,256> freq{};
    for (int i=0;i<256;++i) freq[i] = readU32(in);

    auto root = buildTree(freq);
    // Decode
    uint64_t produced = 0;
    Huffman::Node* node = root.get();

    int bytev;
    int bitpos = 8; // force initial reload
    uint8_t cur = 0;
    auto next_bit = [&]() -> int {
        if (bitpos == 8) {
            bytev = in.get();
            if (bytev == EOF) return -1;
            cur = static_cast<uint8_t>(bytev);
            bitpos = 0;
        }
        int bit = (cur >> bitpos) & 1;
        bitpos++;
        return bit;
    };

    while (produced < orig_size) {
        int bit = next_bit();
        if (bit < 0) break; // unexpected EOF
        node = bit == 0 ? node->left.get() : node->right.get();
        if (!node) throw std::runtime_error("Corrupt bitstream (invalid traversal)");
        if (node->symbol >= 0) {
            out.put(static_cast<char>(uint8_t(node->symbol)));
            produced++;
            node = root.get();
        }
    }
    if (produced != orig_size) throw std::runtime_error("Size mismatch after Huffman decode");
}