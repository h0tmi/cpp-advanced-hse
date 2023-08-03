#include <iostream>
#include <stdexcept>

void Foo() noexcept {
    throw std::runtime_error("Ha-ha!");
}

int main() {
    try {
        Foo();
    } catch (...) {
        std::cout << "Caught something!\n";
    }
}
