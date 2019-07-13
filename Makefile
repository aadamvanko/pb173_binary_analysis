CC = clang
CXX = clang++
CXXFLAGS ?= -std=c++17 -O2 -g

decode:
	$(CXX) $(CXXFLAGS) -o decode hw01/main.cpp
	
cfg:
	$(CXX) $(CXXFLAGS) -o cfg hw02/main.cpp
	
elf: decode.elf cfg.elf

decode.elf:
	$(CXX) $(CXXFLAGS) -o decode.elf hw03/decode-elf/main.cpp
	
cfg.elf:
	$(CXX) $(CXXFLAGS) -o cfg.elf hw03/cfg-elf/main.cpp

symtab:
	$(CXX) $(CXXFLAGS) -o symtab hw04/main.cpp

recfun:
	$(CXX) $(CXXFLAGS) -o recfun hw05/main.cpp
all: decode cfg elf decode.elf cfg.elf symtab recfun
clean:
	rm -f decode cfg elf decode.elf cfg.elf symtab recfun
