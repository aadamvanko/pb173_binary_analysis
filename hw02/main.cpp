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
        string concatenatedBytes;
        string byte;
        while (cin >> byte) {
            concatenatedBytes += (concatenatedBytes.empty() ? "" : " ") + byte;
        }
        instructions = decoder.decodeInstructions(concatenatedBytes);
    }

    /*
    instructions = {
            { "nop0", {}, {}, 0 },
            { "nop1", {}, {}, 0 },
            { "nop2", {}, {}, 0 },
            { "jb", { "", -3, false }, {}, 0 },
            { "je", { "", 2, false }, {}, 0 },
            { "nop3", {}, {}, 0 },
            { "nop4", {}, {}, 0 },
            { "nop5", {}, {}, 0 },
            { "nop6", {}, {}, 0 },
            { "jmp", { "", 3, false }, {}, 0 },
            { "nop8", {}, {}, 0 },
            { "nop9", {}, {}, 0 },
            { "nop10", {}, {}, 0 },
            { "nop11", {}, {}, 0 },
            { "jb", { "", -8, false }, {}, 0 },
            { "nop12", {}, {}, 0 },
            { "nop13", {}, {}, 0 },
            { "nop14", {}, {}, 0 },
            { "call", { "0x2(%rip)", 2, false }, {}, 0 },
            { "nop15", {}, {}, 0 },
            { "nop16", {}, {}, 0 },
            { "jb", { "", -1, false }, {}, 0 },

    };
    */

    ControlFlowGraph controlFlowGraph;
    string dotSource = controlFlowGraph.generateDotSource(instructions);
    cout << dotSource << endl;

    return 0;
}