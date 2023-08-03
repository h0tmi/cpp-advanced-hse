#include <iostream>
#include <string>
#include <stdexcept>

class Holder {
public:
    Holder(std::string value) : value_(std::move(value)) {
        std::cerr << "Holder( " << value_ << " )\n";
    }

    ~Holder() noexcept(false)  // uncomment and try again, it will catch it
    {
        std::cerr << "~Holder( " << value_ << " )\n";
        throw std::runtime_error("Try to catch me");
    }

private:
    std::string value_;
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
        // Catches a destructor exception if it is marked noexcept(false).
        std::cerr << "Caught!\n";
    }

    try {
        try {
            Hopelessness();
        } catch (std::runtime_error& e) {
            // Tried to catch the expected "first exception", but due to
            // an exception inside ~Holder, it will again throw
            // an exception during stack unwinding.

            // In this case, the C++ language guarantees that
            // if it gets an exception during stack unwinding,
            // it will call the terminate() method right now.
            std::cerr << "Caught " << e.what() << "!\n";
        }
    } catch (std::runtime_error& e) {
        std::cerr << "Caught " << e.what() << "!\n";
    }
}
