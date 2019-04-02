cmake_minimum_required(VERSION 3.8)
project(decode-elf)

set(CMAKE_CXX_STANDARD 17)

add_executable(decode.elf main.cpp ../elf_parser.hpp)