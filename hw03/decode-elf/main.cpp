#include <iostream>
#include <fstream>
#include "../../hw01/decode.hpp"
#include "../elf_parser.hpp"

int main(int argc, char* argv[]) {
    using ELFParsing::ELFParser;
    using ELFParsing::Section;
    using ELFParsing::AddressableInstruction;
    using InstructionDecoding::Instruction;
    using InstructionDecoding::Decoder;
    using std::string;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::cin;
/*
    ELFParser elfParser("../test-binaries/a.out");
    const auto addressableInstructions = elfParser.decodeTextSection();
    for (const auto& instruction : addressableInstructions) {
        cout << instruction << endl;
    }
*/
    if (argc == 1) {
        cerr << "Missing argument file" << endl;
        return 1;
    }

    string filename = argv[1];
    vector<AddressableInstruction> addressableInstructions;
    if (ELFParser::IsELFFile(filename)) {
        ELFParser elfParser(filename);
        addressableInstructions = elfParser.decodeTextSection();
    } else {
        uint8_t byte;
        std::vector<uint8_t> bytes;
        std::ifstream file(filename);

        if (!file) {
            cerr << "Cannot open file " << filename << endl;
            return 1;
        }

        while (file >> byte) {
            bytes.push_back(byte);
        }
        Decoder decoder;
        const auto instructions = decoder.decodeInstructions(bytes);
        addressableInstructions = decoder.generateAddressableInstructions(instructions);
    }

    for (const auto& instruction : addressableInstructions) {
        cout << instruction;
        if (instruction.ins.mnemonic == "call") {
            cout << " # " << instruction.destination;
        }
        cout << endl;
    }

    return 0;
}
