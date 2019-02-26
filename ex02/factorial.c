int factorial(int n) {
    if (n <= 0 ) {
        return 1;
    }
    
    int res = factorial(n - 1) * n;
    return res;
}

int main(void) {
    return factorial(5);
}



