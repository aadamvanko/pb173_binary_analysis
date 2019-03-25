#include <iostream>
#include <vector>
#include <string>

#include "cfg.hpp"
#include "../hw01/decode.hpp"

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
        std::vector<uint8_t> bytes;
        uint8_t byte;
        while (cin >> byte) {
            bytes.push_back(byte);
        }
        instructions = decoder.decodeInstructions(bytes);
    }

    ControlFlowGraph controlFlowGraph;
    string dotSource = controlFlowGraph.generateDotSource(instructions);
    cout << dotSource << endl;

    return 0;
}