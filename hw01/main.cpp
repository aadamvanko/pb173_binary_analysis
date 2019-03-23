#include <iostream>
#include <sstream>

#include "decode.hpp"

std::string toHex(int byte) {
    std::ostringstream os;
    os << std::hex << byte;
    std::string s = os.str();
    return os.str();
}

int main(int argc, char **argv) {
    if (argc == 1) {
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
    std::cout << InstructionDecoding::Decoder().decode(concatenatedBytes) << std::endl;

/*
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
    std::cout << decoder.decode("5c") << std::endl;
    std::cout << decoder.decode("5f") << std::endl;
    std::cout << decoder.decode("5e") << std::endl;

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

    std::cout << decoder.decode("8f 04 25 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 04 25 00 00 00 00") << std::endl;
    std::cout << decoder.decode("8f 04 24") << std::endl;

    std::cout << decoder.decode("8f 04 25 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("8f 04 25 00 00 00 00") << std::endl;

    // push imm64

    std::cout << decoder.decode("68 00 00 00 ab") << std::endl;

    // push reg64

    std::cout << decoder.decode("50") << std::endl;
    std::cout << decoder.decode("51") << std::endl;
    std::cout << decoder.decode("52") << std::endl;
    std::cout << decoder.decode("53") << std::endl;
    std::cout << decoder.decode("54") << std::endl;
    std::cout << decoder.decode("55") << std::endl;
    std::cout << decoder.decode("56") << std::endl;
    std::cout << decoder.decode("57") << std::endl;

    // push reg/mem64

    std::cout << decoder.decode("ff b0 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("ff b3 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("ff b1 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("ff b2 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("ff 75 00") << std::endl;
    std::cout << decoder.decode("ff 75 01") << std::endl;
    std::cout << decoder.decode("ff b5 00 00 00 ab") << std::endl;

    std::cout << decoder.decode("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decode("ff 35 00 00 00 01") << std::endl;
    std::cout << decoder.decode("ff 34 25 00 00 00 00") << std::endl;
    std::cout << decoder.decode("ff 34 25 00 ef cd ab") << std::endl;

    std::cout << decoder.decode("ff 30") << std::endl;
    std::cout << decoder.decode("ff 33") << std::endl;
    std::cout << decoder.decode("ff 31") << std::endl;
    std::cout << decoder.decode("ff 32") << std::endl;
    std::cout << decoder.decode("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decode("ff 34 24") << std::endl;
    std::cout << decoder.decode("ff 75 00") << std::endl;
    std::cout << decoder.decode("ff 36") << std::endl;
    std::cout << decoder.decode("ff 37") << std::endl;

    std::cout << decoder.decode("ff 34 25 00 ef cd ab") << std::endl;

    // mov
    std::cout << decoder.decode("48 89 c0") << std::endl;
    std::cout << decoder.decode("48 89 c3") << std::endl;
    std::cout << decoder.decode("48 89 c1") << std::endl;
    std::cout << decoder.decode("48 89 c2") << std::endl;
    std::cout << decoder.decode("48 89 d8") << std::endl;
    std::cout << decoder.decode("48 89 c8") << std::endl;
    std::cout << decoder.decode("48 89 d0") << std::endl;

    std::cout << decoder.decode("48 8b 00") << std::endl;
    std::cout << decoder.decode("48 8b 18") << std::endl;
    std::cout << decoder.decode("48 8b 08") << std::endl;
    std::cout << decoder.decode("48 8b 10") << std::endl;

    std::cout << decoder.decode("48 89 00") << std::endl;
    std::cout << decoder.decode("48 89 03") << std::endl;
    std::cout << decoder.decode("48 89 01") << std::endl;
    std::cout << decoder.decode("48 89 02") << std::endl;

    std::cout << decoder.decode("48 89 05 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 89 85 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 89 1d 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 89 9d 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 8b 05 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 8b 85 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 8b 1d 00 00 00 ab") << std::endl;
    std::cout << decoder.decode("48 8b 9d 00 00 00 ab") << std::endl;
*/

    return 0;
}