#include <stdio.h>

struct __attribute__((packed)) d
{
    short x;
    int y;
};

int main(void) {
    FILE* file = fopen("file.bin", "rb");
    if (!file) { 
        puts("Cannot open file");
        return 1;
    }
    
    struct d data;
    fread(&data, sizeof(struct d), 1, file);
    printf("x = %d, y = %d\n", data.x, data.y);
    
    fclose(file);
    return 0;    
}
