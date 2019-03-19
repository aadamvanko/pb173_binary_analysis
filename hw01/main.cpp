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

    std::cout << decoder.decode("90") << std::endl;

    std::cout << decoder.decode("cc") << std::endl;

    std::cout << decoder.decode("eb ab") << std::endl;
    std::cout << decoder.decode("e9 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("74 ab") << std::endl;
    std::cout << decoder.decode("75 ab") << std::endl;
    std::cout << decoder.decode("72 ab") << std::endl;

    std::cout << decoder.decode("0f 84 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("0f 85 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("0f 82 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("c3") << std::endl;
    std::cout << decoder.decode("c2 00 ab") << std::endl;

    std::cout << decoder.decode("e8 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("58") << std::endl;
    std::cout << decoder.decode("5b") << std::endl;
    std::cout << decoder.decode("59") << std::endl;
    std::cout << decoder.decode("5a") << std::endl;
    std::cout << decoder.decode("5d") << std::endl;

    std::cout << decoder.decode("8f 00") << std::endl;
    std::cout << decoder.decode("8f 03") << std::endl;
    std::cout << decoder.decode("8f 01") << std::endl;
    std::cout << decoder.decode("8f 02") << std::endl;
    std::cout << decoder.decode("8f 45 00") << std::endl;
    std::cout << decoder.decode("8f 45 01") << std::endl;

    std::cout << decoder.decode("8f 80 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 83 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 81 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 82 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 85 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("8f c0") << std::endl;
    std::cout << decoder.decode("8f c3") << std::endl;
    std::cout << decoder.decode("8f c1") << std::endl;
    std::cout << decoder.decode("8f c2") << std::endl;
    std::cout << decoder.decode("8f c5") << std::endl;

    std::cout << decoder.decode("8f 05 00 00 00 00") << std::endl;
    std::cout << decoder.decode("8f 05 00 00 00 01") << std::endl;
    std::cout << decoder.decode("8f 05 00 00 00 ab") << std::endl;

    return 0;
}