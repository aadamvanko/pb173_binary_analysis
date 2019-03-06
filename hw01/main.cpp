#include <iostream>

#include "decode.hpp"

int main(int argc, char **argv) {
    /*if (argc == 1) {
        std::cout << "Too few arguments, missing bytes to decode!" << std::endl;
        return -1;
    }

    std::string concatenatedBytes;
    for (int i = 1; i < argc; i++) {
        if (i != 1) {
            concatenatedBytes += " ";
        }
        concatenatedBytes += std::string(argv[i]);
    }
    */
    std::string concatenatedBytes = "48 89 e5";
    std::cout << InstructionDecoding::Decoder().decode(concatenatedBytes);
    return 0;
}