#include <iostream>
#include "../../hw01/decode.hpp"
#include "../elf_parser.hpp"

int main(int argc, char* argv[]) {
    using ELFParsing::ELFParser;
    using ELFParsing::Section;
    using ELFParsing::AddressableInstruction;
    using InstructionDecoding::Instruction;
    using std::string;
    using std::cout;
    using std::endl;
    using std::cin;

    Instruction mov = { "mov", { "%rax", 0, true }, { "%rbx", 0, true } };
    std::vector<AddressableInstruction> addressableInstructions
    {
        { mov, 0 },
        { mov, 7 },
        { mov, 127 },
        { mov, 2845 },
        { mov, static_cast<uint64_t>(-1) },
        { mov, 2123123123 }
    };
    /*if (argc > 1) {
        instructions = decoder.decodeInstructions(std::vector<const char*>(argv + 1, argv + argc));
    }
    else {
        string concatenatedBytes;
        string byte;
        while (cin >> byte) {
            concatenatedBytes += (concatenatedBytes.empty() ? "" : " ") + byte;
        }
        instructions = decoder.decodeInstructions(concatenatedBytes);
    }*/

    for (const auto& instruction : addressableInstructions) {
        cout << instruction << endl;
    }

    return 0;
}
