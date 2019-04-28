#include <iostream>
#include <fstream>
#include "../../hw01/decode.hpp"
#include "../elf_parser.hpp"
#include "../../hw02/cfg.hpp"

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

    if (argc == 1) {
        cerr << "Missing argument file" << endl;
        return 1;
    }

    ControlFlowGraph controlFlowGraph;

    const string filename = argv[1];
    //const string filename = "../test-binaries/raw_bytes_text_section2.txt";
    //const string filename = "../test-binaries/test_binary";
    if (ELFParser::IsELFFile(filename)) {
        ELFParser elfParser(filename);
        const auto addressableInstructions = elfParser.decodeTextSection();
        cout << controlFlowGraph.generateDotSource(addressableInstructions) << endl;
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
        const auto addressableInstructions = decoder.generateAddressableInstructions(instructions);
        cout << controlFlowGraph.generateDotSource(addressableInstructions) << endl;
    }

    return 0;
}
