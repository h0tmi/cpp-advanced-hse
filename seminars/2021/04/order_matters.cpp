#include <iostream>
#include <stdexcept>

void Foo() {
    throw std::runtime_error("Catch me");
}

int main() {
    try {
        Foo();
    } catch (std::exception&) {
        // do nothing
        std::cout << "Caught std::exception\n";
    } catch (std::runtime_error& e) {
        // will never be here
        std::cout << e.what() << '\n';
    } catch (int error_code) {
    }

    try {
        Foo();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    } catch (int error_code) {
    } catch (...) {
    }
}
