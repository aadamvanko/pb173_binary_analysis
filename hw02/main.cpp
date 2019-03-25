#include <iostream>

#include "cfg.hpp"
#include "../hw01/decode.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string concatenatedBytes;
        for (int i = 1; i < argc; i++) {
            if (i != 1) {
                concatenatedBytes += " ";
            }
            concatenatedBytes += std::string(argv[i]);
        }
        std::cout << InstructionDecoding::Decoder().decode(concatenatedBytes) << std::endl;
    }
    else {
        std::vector<uint8_t> bytes;
        uint8_t byte;
        while (std::cin >> byte) {
            bytes.push_back(byte);
        }
    }

    return 0;
}