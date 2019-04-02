cmake_minimum_required(VERSION 3.8)
project(cfg-elf)

set(CMAKE_CXX_STANDARD 17)

add_executable(cfg.elf main.cpp ../elf_parser.hpp)