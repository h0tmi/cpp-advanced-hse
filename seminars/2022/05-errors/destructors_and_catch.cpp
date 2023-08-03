#include <iostream>
#include <stdexcept>

class IntHolder {
public:
    IntHolder(int value) : ptr_(new int{value}) {
        std::cerr << "IntHolder(" << value << ")\n";
    }
    ~IntHolder() {
        std::cerr << "~IntHolder()\n";
        delete ptr_;
    }

private:
    int* ptr_;
};

void MakeHolder(int input) {
    IntHolder holder{input};
    throw std::runtime_error("Let's try memory leaks");
}

void Wrapper() {
    try {
        MakeHolder(42);
    } catch (std::bad_exception& e) {
    }
}

int main() {
    int input{0};
    try {
        MakeHolder(input);
    } catch (std::logic_error& e) {
        // The destructor will only be called if an error is caught.
        // In that case it will just terminates right now.
    }
    // uncomment the lines below to show the destructor call
    catch (std::runtime_error& e) {
        // There will be no leaks because ~IntHolder will be called.
    }

    // Nested catch.
    // bad_exception not suitable so it will be ignored.
    try {
        Wrapper();
    } catch (std::runtime_error& e) {
        std::cerr << "Caught!\n";
    }
}
