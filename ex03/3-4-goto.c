int fib(int n) {
    int a = 0;
    int b = 1;
    int c = 0;
    while_start:
        c = a + b;
        a = b;
        b = c;
        n--;
        if (n > 0)
            goto while_start;
    return a;
}

int main(void) {
    return fib(10);
}