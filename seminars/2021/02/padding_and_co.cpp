#include <iostream>

struct A {
    char f1;
    char p1[3];
    int f2;  // Member with the largest alignment.
    short f3;
    char p3[2];  // sizeof(A) has to be divisible by the largest alignment of any member.
};

struct B {
    char f1;
    char f2;
    long double f3;
};

struct C {
    char f1;
    long double f2;
    char f3;
};

int main() {
    printf("A: size = %zu; alignment = %zu\n", sizeof(A), alignof(A));
    printf("B: size = %zu; alignment = %zu\n", sizeof(B), alignof(B));
    printf("C: size = %zu; alignment = %zu\n", sizeof(C), alignof(C));
}