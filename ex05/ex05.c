#include <elf.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
    int fd = open("../ex03/3-1", O_RDONLY);

    if (fd == -1) {
        puts("open failed");
        return -1;
    }

    Elf64_Ehdr *p = mmap(NULL, sizeof(Elf64_Ehdr), PROT_READ, MAP_PRIVATE, fd, 0);

    if (p == MAP_FAILED) {
        puts("mmap failed");
        return -1;
    }

    printf("%d\n", p->e_shnum);
    return 0;
}
