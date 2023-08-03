#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>

static int x;

struct Holder {
    std::string value;

    Holder() {
    }
    Holder(std::string value) : value(std::move(value)) {
        ++x;
    }
    Holder(const Holder& h) : value(h.value) {
        std::cout << "Holder(const Holder& h)\n";
        ++x;
        if (x == 2) {
            throw std::runtime_error("Invisible exception");
        }
    }

    Holder(Holder&& h) noexcept : value(std::move(h.value)) {
        std::cout << "Holder(Holder&& h)\n";
        ++x;
    }

    Holder& operator=(const Holder& h) {
        std::cout << "Holder copy assignment operator\n";
        value = h.value;
        return *this;
    }

    Holder& operator=(Holder&& h) noexcept {
        std::cout << "Holder move assignment operator\n";
        value = std::move(h.value);
        return *this;
    }

    ~Holder() = default;
};

struct ThrowHolder {
    std::string value;

    ThrowHolder() {
    }
    ThrowHolder(std::string value) : value(std::move(value)) {
        ++x;
    }
    ThrowHolder(const ThrowHolder& h) : value(h.value) {
        std::cout << "ThrowHolder(const ThrowHolder& h)\n";
        ++x;
        if (x == 2) {
            throw std::runtime_error("Catch me");
        }
    }

    ThrowHolder(ThrowHolder&& h) : value(std::move(h.value)) {
        std::cout << "ThrowHolder(ThrowHolder&& h)\n";
        ++x;
    }

    ThrowHolder& operator=(const ThrowHolder& h) {
        std::cout << "ThrowHolder copy assignment operator\n";
        value = h.value;
        return *this;
    }

    ThrowHolder& operator=(ThrowHolder&& h) {
        std::cout << "ThrowHolder move assignment operator\n";
        value = std::move(h.value);
        return *this;
    }

    ~ThrowHolder() = default;
};

int main() {
    x = 0;
    std::vector<Holder> data;
    data.emplace_back("1");

    try {
        // Call the noexcept constructor if possible.
        data.resize(10);
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }

    x = 0;
    std::vector<ThrowHolder> throw_data;
    throw_data.emplace_back("1");

    try {
        // Otherwise, call the copy constructor.
        throw_data.resize(10);
    } catch (std::exception& e) {
        // Here we will catch the exception.
        std::cout << e.what() << '\n';
    }
}
