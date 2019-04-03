#include <elf.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <array>

#define DEBUG(x) cout << #x << " = " << x << endl

using std::string;
using std::cout;
using std::endl;

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

int main(int argc, char *argv[]) {
    const char *filename = "target";
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Cannot open %s\n", filename);
        return -1;
    }

    struct stat fileInfo;
    if (fstat(fd, &fileInfo) == -1) {
        printf("Cannot get size of file %s\n", filename);
        close(fd);
        return -1;
    }

    void * const fileBytes = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileBytes == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        return -1;
    }

    if (!isELFFile(fileBytes)) {
        printf("File %s is not ELF format\n", filename);
        close(fd);
        return 0;
    }

    Elf64_Ehdr* ELFHeader = static_cast<Elf64_Ehdr*>(fileBytes);
    DEBUG(ELFHeader->e_shoff);
    DEBUG(ELFHeader->e_shnum);
    DEBUG(ELFHeader->e_shentsize);
    DEBUG(ELFHeader->e_shstrndx);

    const uint8_t* fileStart = static_cast<uint8_t*>(fileBytes);
    const Elf64_Shdr* stringTableSectionHeader = static_cast<const Elf64_Shdr*>
            ((void*)(fileStart + ELFHeader->e_shoff)) + ELFHeader->e_shstrndx;

    DEBUG(stringTableSectionHeader->sh_addr);
    DEBUG(stringTableSectionHeader->sh_offset);
    DEBUG(stringTableSectionHeader->sh_size);
    DEBUG(stringTableSectionHeader->sh_name);
    DEBUG(stringTableSectionHeader->sh_entsize);



    char* stringTableData = (char*)((void*)(fileStart + stringTableSectionHeader->sh_offset));

    const Elf64_Section* stringTableSection = static_cast<const Elf64_Section*>
            ((void*)(fileStart + stringTableSectionHeader->sh_offset));



    Elf64_Shdr* sectionHeader = (Elf64_Shdr*)((void*)(fileStart + ELFHeader->e_shoff));
    for (int i = 0; i < ELFHeader->e_shnum; i++) {
        DEBUG(i);
        DEBUG(sectionHeader->sh_entsize);
        DEBUG(sectionHeader->sh_name);
        DEBUG(sectionHeader->sh_size);
        DEBUG(sectionHeader->sh_offset);
        DEBUG(sectionHeader->sh_addr);
        char* sectionNameStr = stringTableData + sectionHeader->sh_name;
        DEBUG(sectionNameStr);

        string sectionName(sectionNameStr);
        if (sectionName == ".text") {
            cout << ".text section found" << endl;
            /*uint8_t* byte = (uint8_t*)((void*)(fileStart + sectionHeader->sh_offset));
            printf(".text section address = %p\n", byte);
            for (int j = 0; j < sectionHeader->sh_size; j++) {
                printf("%02x ", *byte);
                byte++;
            }*/
        }

        if (sectionName == ".strtab") {
            cout << ".symtab section found" << endl;
            char* byte = (char*)((void*)(fileStart + sectionHeader->sh_offset));
            for (int j = 0; j < sectionHeader->sh_size; j++) {
                printf("%c", *byte);
                byte++;
            }
        }

        sectionHeader++;
    }




    close(fd);
    return 0;
}
