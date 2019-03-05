int fun(int x) {
    while (x < 1000) {
        x *= 5;
        if (x % 7 == 0)
            break;
        x--;
    }
    return x;
}

int main(void) {
    return fun(10);
}