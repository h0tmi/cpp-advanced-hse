#include <iostream>
#include <stdexcept>

void Foo() {
    throw std::runtime_error("Catch me");
}

void Bar() {
    throw std::range_error("Catch me");
}

int main() {
    try {
        Foo();
    } catch (std::exception&) {
        std::cout << "Caught std::exception\n";
    } catch (std::runtime_error& e) {
        // Exception of type 'std::runtime_error' will be caught by earlier handler.
        std::cout << e.what() << '\n';
    } catch (int error_code) {
        // You can throw any type of object.
    }

    try {
        Foo();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    } catch (...) {
        // '...' handler must be the last handler for its try block.
    }

    try {
        Bar();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    } catch (std::range_error& e) {
        // Doesn't make any sense since it inherits from std::runtime_error.
        // Be careful and check the exception hierarchy.
    }
}
