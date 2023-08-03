#include <iostream>
#include <cassert>

void f(int i, [[maybe_unused]] int j) {
    assert(i < j);
    std::cout << i;
}

int main() {
    f(1, 2);
}
