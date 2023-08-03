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

    // without try-catch
    //    Foo(flag);

    try {
        Foo(flag);
    } catch (...) {
        std::cout << "Catch exception\n";
        return 1;
    }
    std::cout << "No exception\n";
    return 0;
}
