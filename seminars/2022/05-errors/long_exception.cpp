#include <iostream>
#include <string>
#include <stdexcept>

class Holder {
public:
    Holder(std::string value) : value_(std::move(value)) {
        std::cout << "Holder( " << value_ << " )\n";
    }

    ~Holder() {
        std::cout << "~IntHolder( " << value_ << " )\n";
    }

private:
    std::string value_;
};

void Foo() {
    Holder foo{"Foo"};
    throw std::runtime_error("Catch me");
}

void Bar() {
    Holder bar{"Bar"};
    Foo();
}

void Invisible() {
    Bar();

    // Will never be constructed.
    Holder invisible{"invisible"};
}

int main() {
    try {
        Invisible();
    } catch (...) {
        std::cout << "Caught something\n";
    }
}
