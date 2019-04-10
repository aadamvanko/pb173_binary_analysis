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

#include "../hw01/decode.hpp"

namespace ELFParsing {

    #define DEBUG(x) cout << #x << " = " << x << endl

    using InstructionDecoding::Decoder;
    using InstructionDecoding::Instruction;

    using std::string;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::vector;

    using AddressType = uint64_t;

    struct AddressableInstruction {
        Instruction ins;
        AddressType address = -1;

        friend std::ostream& operator<<(std::ostream& os, const AddressableInstruction& instruction);
    };


    std::ostream& operator<<(std::ostream& os, const AddressableInstruction& instruction)
    {
        os << std::setw(16) << std::hex << instruction.address << ":   " << instruction.ins.toStr();
        return os;
    }

    struct Section {
        string name;
        AddressType startAddress;
        AddressType endAddress; // is after the last valid address
        vector<uint8_t> bytes;
    };

    class ELFParser {
    private:
        string filename_;

        bool isELFFile(const void* fileBytes) {
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
                cout << "File " << filename_ << " is not ELF format" << endl;
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

    public:
        ELFParser(string filename) : filename_(filename) {}

        vector<AddressableInstruction> decodeTextSection() {
            const auto sections = getSections();
            vector<AddressableInstruction> addressableInstructions;
            for (const auto& section : sections) {
                if (section.name != ".text") {
                    continue;
                }

                Decoder decoder;
                const auto instructions = decoder.decodeInstructions(section.bytes);
                AddressType offset = 0;
                for (const auto& instruction : instructions) {
                    AddressableInstruction addressableInstruction{ instruction, section.startAddress + offset };
                    addressableInstructions.push_back(addressableInstruction);
                    offset += instruction.length;
                }
            }
            return addressableInstructions;
        }
    };

}
