#include <dlfcn.h>

int main(void) {
    void *handle = dlopen("./libcallprinter.so", RTLD_LOCAL | RTLD_LAZY);
    if (!handle) {
        printf("Cannot open .so file");
        return 1;
    }

    void (*funPtr)(int) = dlsym(handle, "callPrinter");
    (*funPtr)(3);
    dlclose(handle);
    return 0;
}