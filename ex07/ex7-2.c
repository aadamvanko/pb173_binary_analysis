#include <stdio.h>

void foo(int a, double b) {
    int c = a * b;
    c += a / b;
    printf("foo: %d, %lf, %d\n", a, b, c);
}

int main(void) {
    foo(10, 20);
}