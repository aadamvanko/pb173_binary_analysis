#include <stdio.h>
#include <stdint.h>

int function(int n) {
    int a = 7;
    for (int i = 0; i < 256; i++) {
        printf("i = %d, %p\n", i, *((uint64_t*)((char*)&a) + i));
    }
    return 123;
}

int main(void) {
    int result = function(5);
    return 0;
}