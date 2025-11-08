cmake_minimum_required(VERSION 3.10)
project(CompressionSuite CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compress_tool
    main.cpp
    Huffman.cpp
    LZW.cpp
)

# For Windows static runtime (optional)
if (MSVC)
  target_compile_options(compress_tool PRIVATE /W4)
else()
  target_compile_options(compress_tool PRIVATE -Wall -Wextra -O2)
endif()