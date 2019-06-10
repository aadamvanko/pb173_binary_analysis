#include <iostream>
#include <string>

#include "../hw01/decode.hpp"
#include "../hw03/elf_parser.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Wrong number of arguments, usage: ./recfun BINARY_FILE" << endl;
        return 1;
    }

    //ELFParsing::ELFParser elfParser("../hw05/test-binaries/bad_binary");
    ELFParsing::ELFParser elfParser(argv[1]);
    elfParser.decodeTextSectionRecursively();

    return 0;
}
