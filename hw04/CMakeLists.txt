cmake_minimum_required(VERSION 3.8)
project(symtab)

set(CMAKE_CXX_STANDARD 17)

add_executable(symtab main.cpp ../hw03/elf_parser.hpp)
