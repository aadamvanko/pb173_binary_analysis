#include <iostream>
#include <sstream>

#include "decode.hpp"

std::string toHex(int byte) {
    std::ostringstream os;
    os << std::hex << byte;
    std::string s = os.str();
    return os.str();
}

int main(int argc, char* argv[]) {
    using InstructionDecoding::Instruction;
    using std::string;
    using std::cout;
    using std::endl;
    using std::cin;

    InstructionDecoding::Decoder decoder;
    std::vector<Instruction> instructions;
    if (argc > 1) {
        instructions = decoder.decodeInstructions(std::vector<const char*>(argv + 1, argv + argc));
    }
    else {
        string concatenatedBytes;
        string byte;
        while (cin >> byte) {
            concatenatedBytes += (concatenatedBytes.empty() ? "" : " ") + byte;
        }
        instructions = decoder.decodeInstructions(concatenatedBytes);
    }

    for (const auto& instruction : instructions) {
        cout << instruction << endl;
    }
/*
    for (InstructionDecoding::Instruction instruction : InstructionDecoding::Decoder().decodeInstructions(std::vector<const char*>(argv + 1, argv + argc))) {
        std::cout << instruction.toStr() << std::endl;
    }

    std::cout << "----------------------------------------------------";

     InstructionDecoding::Decoder decoder;
    std::cout << decoder.decodeInstructionToStr("35 ab cf 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 35 ab cf 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("05 ab cf 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 05 ab cf 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11000010)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 0f af " + toHex(0b11010000)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 3d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("3d ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000011)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000001)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11000010)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11011000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11001000)) << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 3b " + toHex(0b11010000)) << std::endl;

    std::cout << decoder.decodeInstructionToStr("90") << std::endl;

    std::cout << decoder.decodeInstructionToStr("cc") << std::endl;

    std::cout << decoder.decodeInstructionToStr("eb ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("e9 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("74 ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("75 ab") << std::endl;
    std::cout << decoder.decodeInstructionToStr("72 ab") << std::endl;

    std::cout << decoder.decodeInstructionToStr("0f 84 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("0f 82 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("c2 00 ab") << std::endl;

    std::cout << decoder.decodeInstructionToStr("e8 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("58") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5b") << std::endl;
    std::cout << decoder.decodeInstructionToStr("59") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5a") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5d") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5c") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5f") << std::endl;
    std::cout << decoder.decodeInstructionToStr("5e") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 03") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 02") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 45 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 45 01") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f 80 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 83 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 81 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 82 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 85 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f c0") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c1") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c2") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f c5") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f 05 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 05 00 00 00 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 05 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f 04 25 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 25 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 24") << std::endl;

    std::cout << decoder.decodeInstructionToStr("8f 04 25 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("8f 04 25 00 00 00 00") << std::endl;

    // push imm64

    std::cout << decoder.decodeInstructionToStr("68 ab 00 00 00") << std::endl;

    // push reg64

    std::cout << decoder.decodeInstructionToStr("50") << std::endl;
    std::cout << decoder.decodeInstructionToStr("51") << std::endl;
    std::cout << decoder.decodeInstructionToStr("52") << std::endl;
    std::cout << decoder.decodeInstructionToStr("53") << std::endl;
    std::cout << decoder.decodeInstructionToStr("54") << std::endl;
    std::cout << decoder.decodeInstructionToStr("55") << std::endl;
    std::cout << decoder.decodeInstructionToStr("56") << std::endl;
    std::cout << decoder.decodeInstructionToStr("57") << std::endl;

    // push reg/mem64

    std::cout << decoder.decodeInstructionToStr("ff b0 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b3 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b1 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b2 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 75 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 75 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff b5 ab 00 00 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 35 01 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 25 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 25 ab cd ef 00") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 30") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 33") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 31") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 32") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 35 00 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 34 24") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 75 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 36") << std::endl;
    std::cout << decoder.decodeInstructionToStr("ff 37") << std::endl;

    std::cout << decoder.decodeInstructionToStr("ff 34 25 ab cd ef 00") << std::endl;

    // mov
    std::cout << decoder.decodeInstructionToStr("48 89 c0") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c3") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c1") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c2") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 d8") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 c8") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 d0") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 8b 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 18") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 08") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 10") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 89 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 03") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 01") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 02") << std::endl;

    std::cout << decoder.decodeInstructionToStr("48 89 05 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 1d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 89 9d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 05 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 85 ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 1d ab 00 00 00") << std::endl;
    std::cout << decoder.decodeInstructionToStr("48 8b 9d ab 00 00 00") << std::endl;
*/

    return 0;
}