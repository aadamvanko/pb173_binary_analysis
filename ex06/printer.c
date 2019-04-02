#include <stdio.h>

int printTimes;

void printNTimes(const char* str) {
    for (int i = 0; i < printTimes; i++) {
        puts(str);
    }
} 