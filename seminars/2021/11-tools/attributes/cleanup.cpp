#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static void free_int(int **ptr) {
    free(*ptr);
    std::cout << "cleanup done\n";
}

int main() {
    __attribute__((cleanup(free_int))) int *ptr_one = (int *)malloc(sizeof(int));

    std::cout << "i'm here: " << ptr_one << "\n";

    return 0;
}