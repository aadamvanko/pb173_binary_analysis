#include <stdio.h>
#include <stdint.h>
#include <string.h>

void forbiddenFunction() {
    printf("Entered the forbidden function\n");
}

int function2(int n) {
    int b = 7;
    return 321;
}

int function(int n) {
    int a = 7;
    *((uint64_t*)&a + 2) = 0x4004d0;
    return 123;
}

int main(void) {
    int res = function(5);
    return 0;
}