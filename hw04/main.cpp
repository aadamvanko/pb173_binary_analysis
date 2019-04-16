#include "../hw03/elf_parser.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    using ELFParsing::ELFParser;
    using ELFParsing::Symbol;

    if (argc == 1) {
        cerr << "Missing argument filename!" << endl;
        return 1;
    }

    ELFParser parser(argv[1]);
    auto symbols = parser.getFunctionSymbols();
    for (const auto& symbol : symbols) {
        cout << symbol << endl;
    }

	return 0;
}
