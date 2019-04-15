int countOneNTimes(int n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += 1;
    }
    return res;
}

int countTenNTimes(int n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += countOneNTimes(10);
    }
    return res;
}

int main(void) {
    int thousand = countTenNTimes(100);
    return 0;
}