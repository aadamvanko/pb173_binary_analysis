int fun(int x) {
    while_label:
        x *= 5;
        if (x % 7 == 0)
            goto while_end;
        x--;
        if (x < 1000)
            goto while_label;
    while_end:
        return x;
}

int main(void) {
    return fun(10);
}