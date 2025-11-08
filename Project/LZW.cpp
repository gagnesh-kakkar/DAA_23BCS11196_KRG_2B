#include "LZW.h"
#include <stdexcept>
#include <vector>
#include <cstring>

static void write_u8(std::ostream& out, uint8_t v){ out.put(static_cast<char>(v)); }
static int read_u8(std::istream& in){ int c=in.get(); return c; }

void LZW::writeU64(std::ostream& out, uint64_t v){
    for(int i=0;i<8;++i) out.put(static_cast<char>((v>>(8*i))&0xFF));
}
uint64_t LZW::readU64(std::istream& in){
    uint64_t v=0; for(int i=0;i<8;++i){ int c=in.get(); if(c==EOF) throw std::runtime_error("Unexpected EOF"); v|=(uint64_t(uint8_t(c))<<(8*i)); }
    return v;
}

void LZW::compress(std::istream& in, std::ostream& out){
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    uint64_t orig_size = data.size();

    // Header
    out.write("CMP1",4);
    write_u8(out, 0x02);
    writeU64(out, orig_size);

    // Build initial dictionary for all single bytes
    const int MAX_BITS = 12;
    const int MAX_DICT_SIZE = 1<<MAX_BITS; // 4096
    std::unordered_map<std::string, int> dict;
    dict.reserve(5000);
    for (int i=0;i<256;++i) {
        std::string s(1, char(uint8_t(i)));
        dict[s] = i;
    }
    int next_code = 256;

    // Helper for 12-bit packing
    uint32_t bitbuf = 0;
    int bitcount = 0;
    auto emit_code = [&](int code){
        bitbuf |= (uint32_t(code) << bitcount);
        bitcount += 12;
        while (bitcount >= 8){
            write_u8(out, uint8_t(bitbuf & 0xFF));
            bitbuf >>= 8;
            bitcount -= 8;
        }
    };
    auto flush_codes = [&](){
        if (bitcount > 0) {
            write_u8(out, uint8_t(bitbuf & 0xFF));
            bitbuf = 0; bitcount = 0;
        }
    };

    // LZW encoding
    std::string w;
    for (uint8_t k : data){
        std::string wk = w + char(k);
        if (dict.find(wk) != dict.end()) {
            w = std::move(wk);
        } else {
            emit_code(dict[w]);
            if (next_code < MAX_DICT_SIZE) {
                dict[wk] = next_code++;
            }
            w = std::string(1, char(k));
        }
    }
    if (!w.empty()) emit_code(dict[w]);
    flush_codes();
}

void LZW::decompress(std::istream& in, std::ostream& out){
    char magic[4];
    if (!in.read(magic, 4)) throw std::runtime_error("Bad header");
    if (std::memcmp(magic, "CMP1", 4) != 0) throw std::runtime_error("Not CMP1");
    int algo = in.get();
    if (algo != 0x02) throw std::runtime_error("Wrong algo id for LZW");
    uint64_t orig_size = readU64(in);

    const int MAX_BITS = 12;
    const int MAX_DICT_SIZE = 1<<MAX_BITS; // 4096

    // Initialize dictionary
    std::vector<std::string> dict(MAX_DICT_SIZE);
    for (int i=0;i<256;++i) dict[i] = std::string(1, char(uint8_t(i)));
    int next_code = 256;

    // 12-bit unpacking (LSB-first)
    uint32_t bitbuf = 0;
    int bitcount = 0;
    auto read_code = [&]() -> int {
        while (bitcount < 12){
            int c = in.get();
            if (c == EOF) {
                if (bitcount == 0) return -1;
                // trailing bits leftover is an error for this format
                return -2;
            }
            bitbuf |= (uint32_t(uint8_t(c)) << bitcount);
            bitcount += 8;
        }
        int code = int(bitbuf & 0xFFF);
        bitbuf >>= 12;
        bitcount -= 12;
        return code;
    };

    int code = read_code();
    if (code < 0) return;
    std::string w = dict[code];
    out.write(w.data(), w.size());
    uint64_t produced = w.size();

    int k;
    while ((k = read_code()) >= 0) {
        std::string entry;
        if (k < next_code && !dict[k].empty()) {
            entry = dict[k];
        } else if (k == next_code) {
            // KwKwK case
            entry = w + w[0];
        } else {
            throw std::runtime_error("Corrupt LZW stream");
        }
        out.write(entry.data(), entry.size());
        produced += entry.size();

        if (next_code < MAX_DICT_SIZE) {
            dict[next_code++] = w + entry[0];
        }
        w = std::move(entry);
        if (produced >= orig_size) break;
    }
    if (produced != orig_size) {
        // We allow some containers to end exactly at size.
        if (produced < orig_size) throw std::runtime_error("Size mismatch after LZW decode");
    }
}