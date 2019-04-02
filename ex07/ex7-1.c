void fallAt(int depth) {
    int* p = 0;
    if (depth == 0) {
        *(p) = 777;
    }
    fallAt(depth - 1);
}

int main(void) {
    fallAt(3);
}