#include <iostream>
#include <string>
#include <variant>

auto IsPrime() -> bool;

auto main() -> int {
    auto is_prime = IsPrime();

    const auto prime = std::string{"prime"};

    // Immediately invoked lambda expression
    // IILE
    const auto s1 = is_prime ? prime : "not prime";

    const auto s = [&]() {
        auto something = IsPrime();

        if (is_prime) {
            return prime;
        } else {
            return "not prime";
        }
    }();

    std::string s1;
    if (is_prime) {
        s1 = std::string{"prime"};
    } else {
        s1 = std::string{"not prime"};
    }

    // Use `s` somehow.
    std::cout << s << std::endl;
}
