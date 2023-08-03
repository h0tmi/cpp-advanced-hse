#include <iostream>

struct Empty {};

// These addresses of all fields with the same type have to be distinguishable.
struct A {
    Empty f1;  // 1 byte
    Empty f2;  // 1 byte
};

// In this case empty-base optimization kicks in.
struct B : Empty {  // 0 bytes
    char f1;        // 1 byte
    Empty f2;       // 1 byte
};

// Yet here we have the same problem with addressing!
struct C : Empty {  // 1 byte
    Empty f1;       // 1 byte
    char f2;        // 1 byte
};

template <class T, class Allocator = std::allocator<T>>
struct MyVeryCoolDataStructure {
    T* kek;
    Allocator allocator;
    // A bunch of extra padding.
};

int main() {
    printf("Size of A: %zu\n", sizeof(A));
    printf("Size of B: %zu\n", sizeof(B));
    printf("Size of C: %zu\n", sizeof(C));
    printf("Size of MyVeryCoolDataStructure: %zu\n", sizeof(MyVeryCoolDataStructure<int>));
    static_assert(std::is_empty_v<std::allocator<int>>);
    return 0;
}