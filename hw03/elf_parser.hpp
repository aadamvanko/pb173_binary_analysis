#include <elf.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <algorithm>

#include "../hw01/decode.hpp"

namespace ELFParsing {

    #define DEBUG(x) cout << #x << " = " << x << endl

    using InstructionDecoding::Decoder;
    using InstructionDecoding::Instruction;
    using InstructionDecoding::AddressableInstruction;
    using InstructionDecoding::AddressType;

    using std::string;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::vector;

    struct Section {
        string name;
        AddressType startAddress;
        AddressType endAddress; // is after the last valid address
        vector<uint8_t> bytes;
    };

    struct Symbol {
        string name;
        char type;
        AddressType address;

        friend std::ostream& operator<<(std::ostream& os, const Symbol& symbol);
    };

    std::ostream& operator<<(std::ostream& os, const Symbol& symbol)
    {
        os << std::setfill('0') << std::setw(16) << std::hex << symbol.address << " " << symbol.type << " " << symbol.name;
        return os;
    }

    class ELFParser {
    private:
        string filename_;

        static bool IsELFFile(const string& filename) {
            int fd = open(filename.c_str(), O_RDONLY);
            if (fd == -1) {
                cerr << "Cannot open " << filename << endl;
                return false;
            }

            struct stat fileInfo;
            if (fstat(fd, &fileInfo) == -1) {
                cerr << "Cannot get size of file " << filename << endl;
                close(fd);
                return false;
            }

            void * const fileBytes = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (fileBytes == MAP_FAILED) {
                cerr << "mmap failed" << endl;
                close(fd);
                return false;
            }

            if (!isELFFile(fileBytes)) {
                // cerr << "File " << filename << " is not ELF format" << endl;
                close(fd);
                return false;
            }

            close(fd);
            return true;
        }

        static bool isELFFile(const void* fileBytes) {
            const uint8_t* bytes = static_cast<const uint8_t*>(fileBytes);
            std::array<uint8_t, 4> ELFMagicNumber = { 0x7f, 0x45, 0x4c, 0x46 };
            for (size_t offset = 0; offset < ELFMagicNumber.size(); offset++) {
                if (*(bytes + offset) != ELFMagicNumber[offset]) {
                    return false;
                }
            }
            return true;
        }


        vector<Section> getSections() {
            int fd = open(filename_.c_str(), O_RDONLY);
            if (fd == -1) {
                cerr << "Cannot open " << filename_ << endl;
                return {};
            }

            struct stat fileInfo;
            if (fstat(fd, &fileInfo) == -1) {
                cerr << "Cannot get size of file " << filename_ << endl;
                close(fd);
                return {};
            }

            void * const fileBytes = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (fileBytes == MAP_FAILED) {
                cerr << "mmap failed" << endl;
                close(fd);
                return {};
            }

            if (!isELFFile(fileBytes)) {
                cerr << "File " << filename_ << " is not ELF format" << endl;
                close(fd);
                return {};
            }

            Elf64_Ehdr* ELFHeader = static_cast<Elf64_Ehdr*>(fileBytes);
/*
            DEBUG(ELFHeader->e_shoff);
            DEBUG(ELFHeader->e_shnum);
            DEBUG(ELFHeader->e_shentsize);
            DEBUG(ELFHeader->e_shstrndx);
*/
            const uint8_t* fileStart = static_cast<uint8_t*>(fileBytes);
            const Elf64_Shdr* stringTableSectionHeader = static_cast<const Elf64_Shdr*>
                    ((void*)(fileStart + ELFHeader->e_shoff)) + ELFHeader->e_shstrndx;
/*
            DEBUG(stringTableSectionHeader->sh_addr);
            DEBUG(stringTableSectionHeader->sh_offset);
            DEBUG(stringTableSectionHeader->sh_size);
            DEBUG(stringTableSectionHeader->sh_name);
            DEBUG(stringTableSectionHeader->sh_entsize);
*/
            char* stringTableData = (char*)((void*)(fileStart + stringTableSectionHeader->sh_offset));

            const Elf64_Section* stringTableSection = static_cast<const Elf64_Section*>
                    ((void*)(fileStart + stringTableSectionHeader->sh_offset));

            vector<Section> sections;
            Elf64_Shdr* sectionHeader = (Elf64_Shdr*)((void*)(fileStart + ELFHeader->e_shoff));
            for (int i = 0; i < ELFHeader->e_shnum; i++) {
                char* sectionNameStr = stringTableData + sectionHeader->sh_name;
                uint8_t* bytes = (uint8_t*)((void*)(fileStart + sectionHeader->sh_offset));

                Section section;
                section.name = sectionNameStr;
                section.startAddress = sectionHeader->sh_addr;
                section.endAddress = sectionHeader->sh_addr + sectionHeader->sh_size;
                section.bytes = vector<uint8_t>(bytes, bytes + sectionHeader->sh_size);
                sections.push_back(section);

/*
                DEBUG(i);
                DEBUG(sectionHeader->sh_entsize);
                DEBUG(sectionHeader->sh_name);
                DEBUG(sectionHeader->sh_size);
                DEBUG(sectionHeader->sh_offset);
                DEBUG(sectionHeader->sh_addr);
                DEBUG(sectionNameStr);
                printf("section bytes address = %p\n", bytes);

                if (section.name == ".text") {
                    cout << ".text section found" << endl;
                    printf(".text section bytes address = %p\n", bytes);
                    for (const auto& byte : section.bytes) {
                        printf("%02x ", byte);
                    }
                }

                if (section.name == ".strtab") {
                    cout << ".symtab section found" << endl;
                    char* byte = (char*)((void*)(fileStart + sectionHeader->sh_offset));
                    for (int j = 0; j < sectionHeader->sh_size; j++) {
                        printf("%c", *byte);
                        byte++;
                    }
                }
*/
                sectionHeader++;
            }
            close(fd);

            return sections;
        }

        bool isMovWithRIP(const AddressableInstruction& addrIns) {
            return addrIns.ins.mnemonic == "mov" &&
                    (addrIns.ins.operandA.representation.find("(%rip)") != string::npos ||
                     addrIns.ins.operandB.representation.find("(%rip)") != string::npos);
        }

        AddressType getRIPOffset(const AddressableInstruction& addrIns) {
            if (addrIns.ins.operandA.representation.find("(%rip)")) {
                return addrIns.ins.operandA.value;
            }
            if (addrIns.ins.operandB.representation.find("(%rip)")) {
                return addrIns.ins.operandB.value;
            }
            return -1;
        }

        string generateDestinationAddressComment(const AddressableInstruction& addrIns, const vector<Section>& sections) {
            AddressType destAddress = addrIns.address + getRIPOffset(addrIns);
            for (const auto& section : sections) {
                /*
                if (section.name == ".text") {
                    DEBUG(section.name);
                    DEBUG(section.startAddress);
                    DEBUG(section.endAddress);
                }
                */
                if (section.startAddress <= destAddress && destAddress < section.endAddress) {
                    return section.name + " + " + std::to_string(destAddress - section.startAddress);
                }
            }
            return "unkown section + ...";
        }

        Section getSection(const vector<Section>& sections, const string& sectionName) {
            for (const auto& section : sections) {
                if (section.name == sectionName) {
                    return section;
                }
            }
            throw std::logic_error("Section with given name not found");
        }

    public:
        ELFParser(string filename) : filename_(filename) {}

        vector<AddressableInstruction> decodeTextSection() {
            const auto sections = getSections();
            for (const auto& section : sections) {
                if (section.name != ".text") {
                    continue;
                }

                Decoder decoder;
                const auto instructions = decoder.decodeInstructions(section.bytes);
                const auto addressableInstructions = decoder.generateAddressableInstructions(instructions);
                vector<AddressableInstruction> instructionsWithDestination;

                for (const auto& addressableInstruction : addressableInstructions) {
                    AddressableInstruction instructionWithDestination = addressableInstruction;
                    instructionWithDestination.address += section.startAddress;
                    if (isMovWithRIP(addressableInstruction)) {
                        string destAddressComment = generateDestinationAddressComment(addressableInstruction, sections);
                        instructionWithDestination.comment = destAddressComment;
                    }
                    instructionsWithDestination.push_back(instructionWithDestination);
                }

                return instructionsWithDestination;
            }

            cerr << "Section .text was not found in given ELF file" << endl;
            return {};
        }

        vector<Symbol> getFunctionSymbols() {
            auto sections = getSections();
            auto strtabSection = getSection(sections, ".strtab");
            auto symtabSection = getSection(sections, ".symtab");

            vector<Symbol> symbols;
            Elf64_Sym* symbol = (Elf64_Sym*)&symtabSection.bytes[0];
            while (symbol < (Elf64_Sym*)&symtabSection.bytes[symtabSection.bytes.size() - 1]) {
                if (ELF64_ST_TYPE(symbol->st_info) != STT_FUNC) {
                    symbol++;
                    continue;
                }

                const char* symbolNameStr = (const char*)&strtabSection.bytes[symbol->st_name];
                string symbolName = symbolNameStr;
                if (symbolName != "") {
                    char type = (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL ? 't' : 'T');
                    symbols.push_back( { symbolName, type, symbol->st_value } );
                }
                symbol++;
            }

            std::sort(symbols.begin(), symbols.end(),
                 [](const Symbol& lhs, const Symbol& rhs) { return lhs.address < rhs.address; });

            return symbols;
        }
    };

}
