#include <string>

struct S {
    mutable int calls;
    std::string data;

    auto GetData() const -> const std::string& {
        calls += 1;
        return data;
    }
};

auto main() -> int {
    // Capture by value
    auto foo = 0;
    auto increment = [=] { foo++; };  // Does not compile

    // Capture by reference
    auto bar = 0;
    auto decrement = [&, v = S{}.GetData()](const auto& arg) {
        bar--;
        v.size();
    };
}
