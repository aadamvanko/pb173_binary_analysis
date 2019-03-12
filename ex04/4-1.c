int fun(int n) {
    switch (n) {
        case 1:
            return 11;
        case 2:
            return 21;
        case 3:
            return 31;
        case 4:
            return 41;
        case 5:
            return 51;
        case 6:
            return 61;
        case 7:
            return 71;
        case 8:
            return 81;
        case 9:
            return 91;
        default:
            return 100;
    }
}

int main(void) {
    return fun(5);
}