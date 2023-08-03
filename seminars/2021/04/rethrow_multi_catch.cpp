#include <iostream>
#include <stdexcept>

void Foo() {
    throw std::runtime_error("Catch me and throw me again");
}

int main() {
    try {
        Foo();
    } catch (std::runtime_error& e) {
        // Just call .what() to get the exception message.
        std::cout << e.what() << '\n';

        // rethrow exception. What will happen next?
        throw;
    } catch (...) {
        // will never be here
        std::cout << "Catch exception\n";
    }

    std::cout << "Normal exit\n";
    return 0;
}
