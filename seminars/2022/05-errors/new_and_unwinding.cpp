#include <iostream>
#include <memory>
#include <stdexcept>

void Foo(bool crash_me) {
    // memory will leak
    int* ptr = new int{1};

    // Stack unwinding calls destructors, smart pointers will be fine
    std::shared_ptr<int> shart_ptr = std::make_shared<int>(1);

    if (crash_me) {
        throw std::runtime_error("Thanks :)");
    }

    std::cout << "You will never see me\n";
    delete ptr;
}

int main() {
    bool flag;
    std::cin >> flag;

    // Without try-catch a memory leak will occur if the flag is set to true.
    Foo(flag);

    try {
        Foo(flag);
    } catch (...) {
        // With try-catch there will still be a leak due to line 8.
        std::cout << "Catch exception\n";
        return 1;
    }
    std::cout << "No exception\n";
    return 0;
}
