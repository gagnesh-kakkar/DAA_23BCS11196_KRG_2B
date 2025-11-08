#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <queue>
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>

// Simple Huffman codec for arbitrary byte streams.
// File format used by this project when writing compressed data:
// [magic: 'CMP1'] [algo: 1 byte=0x01 for Huffman]
// [orig_size: uint64 little-endian]
// [freq table: 256 * uint32 little-endian for each byte value]
// [payload: bit-packed codes; padded to full bytes]
//
// Decoding reconstructs the tree from the frequency table and emits
// exactly orig_size bytes by traversing the bitstream.
class Huffman {
public:
    struct Node {
        uint64_t freq;
        int16_t symbol; // -1 for internal node, 0..255 for leaf
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(uint64_t f, int16_t s) : freq(f), symbol(s) {}
        Node(uint64_t f, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
            : freq(f), symbol(-1), left(std::move(l)), right(std::move(r)) {}
    };

    static void compress(std::istream& in, std::ostream& out);
    static void decompress(std::istream& in, std::ostream& out);

private:
    using CodeMap = std::array<std::vector<bool>, 256>;

    static std::unique_ptr<Node> buildTree(const std::array<uint64_t,256>& freq);
    static void buildCodes(const Node* node, std::vector<bool>& path, CodeMap& codes);
    static void writeU32(std::ostream& out, uint32_t v);
    static void writeU64(std::ostream& out, uint64_t v);
    static uint32_t readU32(std::istream& in);
    static uint64_t readU64(std::istream& in);
};