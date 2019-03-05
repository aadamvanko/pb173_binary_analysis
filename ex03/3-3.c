int fib(int n) {
    int result = 1;
    if (n <= 2)
        goto fib_end;
    int a = fib(n - 1);
    int b = fib(n - 2);
    result = a + b;
    fib_end:
        return result;
}

int main(void) {
    return fib(10);
}