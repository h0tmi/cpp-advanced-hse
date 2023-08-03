#include <iostream>
#include <string>
#include <stdexcept>

class Holder {
public:
    Holder(std::string value) : value_(std::move(value)) {
        std::cerr << "Holder( " << value_ << " )\n";
    }

    ~Holder() {
        std::cerr << "~Holder( " << value_ << " )\n";
    }

private:
    std::string value_;
};

class ThrowHolder {
public:
    ThrowHolder(std::string value) : value_(std::move(value)) {
        std::cerr << "ThrowHolder( " << value_ << " )\n";
        throw std::runtime_error("Bad constructor");
    }

    ~ThrowHolder() {
        std::cerr << "~ThrowHolder( " << value_ << " )\n";
    }

private:
    std::string value_;
};

class Pair {
public:
    Pair(std::string value) : h_(value), th_(value) {
        std::cerr << "Pair()\n";
    }

    ~Pair() {
        std::cerr << "~Pair()\n";
    }

private:
    Holder h_;
    ThrowHolder th_;
};

int main() {
    try {
        Pair summer{"summer"};
    } catch (std::runtime_error& e) {
        std::cerr << "Caught!\n";
    }
}
