#include <iostream>
#include <fstream>
#include <string>
#include "Huffman.h"
#include "LZW.h"

// Command-line frontend.
// Usage:
//   compress_tool compress huffman input_file output_file
//   compress_tool compress lzw input_file output_file
//   compress_tool decompress huffman input_file output_file
//   compress_tool decompress lzw input_file output_file
//
// Notes:
//  - Output uses a simple container with a 'CMP1' magic + algo id.
//  - Huffman stores a 256xuint32 frequency table for deterministic decode.
//  - LZW uses 12-bit codes up to 4096 dictionary entries.
//
// Build (with CMake):
//   mkdir build && cd build && cmake .. && cmake --build . -j
// 
// Simple test (Linux/macOS):
//   ./compress_tool compress huffman ../README.md out.cmp
//   ./compress_tool decompress huffman out.cmp restored.md
//
// Error handling is basic to keep the demo compact.

int main(int argc, char** argv){
    if (argc != 5) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " compress huffman|lzw <in> <out>\n"
                  << "  " << argv[0] << " decompress huffman|lzw <in> <out>\n";
        return 1;
    }
    std::string mode = argv[1];
    std::string algo = argv[2];
    std::string inpath = argv[3];
    std::string outpath = argv[4];

    std::ifstream in(inpath, std::ios::binary);
    if (!in) { std::cerr << "Cannot open input file: " << inpath << "\n"; return 1; }
    std::ofstream out(outpath, std::ios::binary);
    if (!out) { std::cerr << "Cannot open output file: " << outpath << "\n"; return 1; }

    try {
        if (mode == "compress") {
            if (algo == "huffman") Huffman::compress(in, out);
            else if (algo == "lzw") LZW::compress(in, out);
            else { std::cerr << "Unknown algorithm\n"; return 2; }
        } else if (mode == "decompress") {
            if (algo == "huffman") Huffman::decompress(in, out);
            else if (algo == "lzw") LZW::decompress(in, out);
            else { std::cerr << "Unknown algorithm\n"; return 2; }
        } else {
            std::cerr << "Unknown mode\n"; return 2;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 3;
    }
    return 0;
}