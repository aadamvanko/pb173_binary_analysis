#include <stdio.h>

void foo(int a, double b) {
    printf("foo: %d, %lf\n", a, b);
}

void bar(double a, int b) {
    printf("bar: %lf, %d\n", a, b);
}

int main(void) {
    foo(14, 3.14);
    bar(3.14, 14);
}