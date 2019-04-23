#include <stdio.h>

extern long int mymax(long int a, long int b);

int main(void) {
    long int bigger = mymax(10, 20);
    printf("bigger is %ld\n", bigger);
    bigger = mymax(20, 10);
    printf("bigger is %ld\n", bigger);
    return 0;
}