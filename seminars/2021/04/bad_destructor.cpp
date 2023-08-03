#include <iostream>
#include <string>
#include <stdexcept>

class Holder {
public:
    Holder(std::string value_) : value(std::move(value_)) {
        std::cout << "Holder( " << value << " )\n";
    }

    ~Holder()  // noexcept(false) // uncomment and try again, it will catch it
    {
        std::cout << "~Holder( " << value << " )\n";
        throw std::runtime_error("Try to catch me");
    }

private:
    std::string value;
};

void Foo() {
    Holder foo{"Foo"};
}

void Hopelessness() {
    Holder hope{"hopelessness"};
    throw std::runtime_error("First exception");
}

int main() {
    try {
        Foo();
    } catch (std::runtime_error& e) {
        std::cout << "Caught!\n";
    }

    try {
        try {
            Hopelessness();
        } catch (std::runtime_error& e) {
            std::cout << "Caught " << e.what() << "!\n";
        }
    } catch (std::runtime_error& e) {
        std::cout << "Caught " << e.what() << "!\n";
    }
}
