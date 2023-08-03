#include "test_type.h"

#include <iostream>
#include <memory>

// Mistakes happen, especially with branching conditions.
bool DoWork() {
    auto p = new int(5);
    if (*p == 2) {
        delete p;
        return true;
    }
    delete p;
    return false;
}

void DoSomethingThatMightThrow() {
    throw std::runtime_error("I am an error!");
}

int main() {
    try {
        auto ptr = new Test("ex");
        DoSomethingThatMightThrow();
        delete ptr;
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
    }
}
