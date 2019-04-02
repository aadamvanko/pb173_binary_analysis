#include <iostream>
#include "../../hw01/decode.hpp"

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

    return 0;
}
