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
    Holder(std::string value_) : value(std::move(value_)) {
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
        std::cout << "Copy assignment operator\n";
        value = h.value;
        return *this;
    }

    Holder& operator=(Holder&& h) noexcept {
        std::cout << "Move assignment operator\n";
        value = std::move(h.value);
        return *this;
    }

    ~Holder() = default;
};

struct ThrowHolder {
    std::string value;

    ThrowHolder() {
    }
    ThrowHolder(std::string value_) : value(std::move(value_)) {
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

    ThrowHolder& operator=(const Holder& h) {
        std::cout << "Copy assignment operator\n";
        value = h.value;
        return *this;
    }

    ThrowHolder& operator=(Holder&& h) {
        std::cout << "Move assignment operator\n";
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
        data.resize(10);
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }

    x = 0;
    std::vector<ThrowHolder> throw_data;
    throw_data.emplace_back("1");

    try {
        throw_data.resize(10);
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}
