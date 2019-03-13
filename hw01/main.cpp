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
    InstructionDecoding::Decoder decoder;
    std::cout << decoder.decode("35 00 00 ab cf") << std::endl;
    std::cout << decoder.decode("48 35 00 00 ab cf") << std::endl;
    std::cout << decoder.decode("05 00 00 ab cf") << std::endl;
    std::cout << decoder.decode("48 05 00 00 ab cf") << std::endl;
    return 0;
}