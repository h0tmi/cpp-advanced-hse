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
    } catch (std::exception e) {  // catch by value
        std::cout << "exception caught in a(): " << e.what() << std::endl;
        throw;  // rethrow
    }
}

int main() {
    try {
        Foo();
    } catch (const SpecialException& e) {  // catch by ref
        std::cout << "exception caught in main(): " << e.what() << std::endl;
    }
}
