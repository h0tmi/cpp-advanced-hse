#include <ios>
#include <system_error>
#include <stdexcept>
#include <iostream>
#include <string>

class SpecialException : public std::exception {
public:
    const char* what() const noexcept {
        return "SpecialException";
    }
};

void Foo() {
    try {
        throw SpecialException();
        // When you catch by value, the what() method of the caught type is called.
        // Although the original exception will be re-thrown.
    } catch (std::exception e) {  // catch by value
        std::cerr << "exception caught in a(): " << e.what() << std::endl;
        throw;  // rethrow
    }
}

int main() {
    try {
        Foo();
        // Therefore, it is better to catch by reference.
    } catch (const std::exception& e) {
        std::cerr << "exception caught in main(): " << e.what() << std::endl;
    }
}
