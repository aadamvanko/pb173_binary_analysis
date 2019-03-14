#include <iostream>
#include <string>
#include <sstream>

#include "decode.hpp"

std::string toHex(int byte) {
    std::ostringstream os;
    os << std::hex << byte;
    std::string s = os.str();
    return os.str();
}

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
    std::cout << decoder.decode("48 0f af " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11000010)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decode("48 0f af " + toHex(0b11010000)) << std::endl;
    std::cout << decoder.decode("48 3d 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("3d 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11000010)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decode("48 3b " + toHex(0b11010000)) << std::endl;
    return 0;
}