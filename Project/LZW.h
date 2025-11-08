#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <istream>
#include <ostream>

// LZW implementation using 12-bit codes (max dictionary size 4096).
// File format when writing:
// [magic: 'CMP1'] [algo: 1 byte=0x02 for LZW]
// [orig_size: uint64 little-endian]
// [payload: stream of 12-bit codes packed into bytes, LSB-first]
class LZW {
public:
    static void compress(std::istream& in, std::ostream& out);
    static void decompress(std::istream& in, std::ostream& out);

private:
    static void writeU64(std::ostream& out, uint64_t v);
    static uint64_t readU64(std::istream& in);
};