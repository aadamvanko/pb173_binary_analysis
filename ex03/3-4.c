int fib(int n) {
    int a = 0;
    int b = 1;
    int c = 0;
    while (n > 0) {
        c = a + b;
        a = b;
        b = c;
        n--;
    }
    return a;
}

int main(void) {
    return fib(10);
}