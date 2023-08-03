#include "test_type.h"

#include <iostream>
#include <memory>

int main() {
    auto ptr = std::make_shared<Test>("ex");
    {
        auto ptr1 = ptr;
        // Tracks the amount of copies.
        std::cout << ptr.use_count() << "\n";
    }
    std::cout << ptr.use_count() << "\n";
    // Non-owning pointer that allows checking whether the object is still alive.
    std::weak_ptr<Test> weak = ptr;
    if (auto ptr2 = weak.lock()) {
        std::cout << ptr2->Get() << "\n";
    }
    ptr.reset();

    if (auto ptr2 = weak.lock()) {
        std::cout << ptr2->Get() << "\n";
    } else {
        std::cout << "Pointer already died\n";
    }
}
