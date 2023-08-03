#include "test_type.h"

#include <iostream>
#include <memory>

// Much harder to make any accidental mistakes.
bool DoWork() {
    auto p = std::make_unique<int>(5);
    if (*p == 2) {
        return true;
    }
    return false;
}

void DoSomethingThatMightThrow() {
    throw std::runtime_error("I am an error!");
}

int main() {
    // No memory leak anymore.
    try {
        auto ptr = std::make_unique<Test>("ex");
        DoSomethingThatMightThrow();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
    }
}
