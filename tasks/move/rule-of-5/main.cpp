#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

std::string LimitedGetline() {
    constexpr int kMaxSize = 80;
    std::string line;
    while (std::cin && line.size() < kMaxSize) {
        int symbol = std::cin.get();
        if (symbol == '\n') {
            break;
        }
        line.push_back(symbol);
    }
    return line;
}

// This class is following rule of 5.
// http://en.cppreference.com/w/cpp/language/rule_of_three
class SlowNumber {
public:
    SlowNumber(int value) : value_(new int(value)) {
    }

    SlowNumber(SlowNumber&& other) : value_(other.value_) {
        other.value_ = nullptr;
    }

    SlowNumber(const SlowNumber& other) : value_(new int(*other.value_)) {
    }

    SlowNumber& operator=(SlowNumber&& other) {
        if (value_) {
            delete value_;
        }

        value_ = other.value_;
        other.value_ = nullptr;
        return *this;
    }

    SlowNumber& operator=(const SlowNumber& other) {
        if (value_) {
            delete value_;
            value_ = nullptr;
        }

        if (other.value_) {
            value_ = new int(*other.value_);
        }
        return *this;
    }

    ~SlowNumber() {
        if (value_) {
            delete value_;
        }
    }

    int Value() {
        return *value_;
    }

private:
    int* value_ = nullptr;
};

int main() {
    std::vector<std::shared_ptr<SlowNumber>> stack;

    for (int i = 0; i < 1000; ++i) {
        auto cmd = LimitedGetline();
        if (cmd == "push") {
            int value = 0;
            std::cin >> value;
            std::cin.get();  // eat up newline
            stack.emplace_back(std::make_shared<SlowNumber>(value));
        } else if (cmd == "pop") {
            if (!stack.empty()) {
                stack.pop_back();
            } else {
                std::cout << "error: stack is empty" << std::endl;
            }
        } else if (cmd == "print") {
            for (auto& v : stack) {
                std::cout << v->Value() << std::endl;
            }
        } else if (cmd == "swap") {
            size_t i = 0;
            size_t j = 0;
            std::cin >> i >> j;
            std::cin.get();  // eat up newline

            if (i < 0 || i >= stack.size() || j < 0 || j >= stack.size()) {
                std::cout << "error: invalid index" << std::endl;
                continue;
            }

            std::swap(*(stack[i]), *(stack[j]));
        } else if (cmd == "dup") {
            if (stack.empty()) {
                std::cout << "error: stack is empty" << std::endl;
                continue;
            }

            stack.push_back(stack.back());
        } else if (cmd == "pop_carry") {
            if (stack.size() < 2) {
                std::cout << "error: stack size must be greater than 2" << std::endl;
                continue;
            }

            *(stack[stack.size() - 2]) = std::move(*stack.back());
            stack.pop_back();
        } else if (cmd == "") {
            break;
        } else {
            std::cout << "unknown cmd" << std::endl;
        }
    }

    return 0;
}
