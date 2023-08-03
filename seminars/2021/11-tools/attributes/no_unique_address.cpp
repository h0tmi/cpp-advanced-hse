#include <iostream>

struct Empty {};

struct X {
    int i;
    Empty e;
};

struct Y {
    int i;
    [[no_unique_address]] Empty e;
};

int main() {
    std::cout << sizeof(Empty) << '\n';

    std::cout << sizeof(X) << '\n';

    std::cout << sizeof(Y) << '\n';
}