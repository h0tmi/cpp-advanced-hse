#include <mpark/patterns.hpp>

#include <iostream>

void fizzbuzz() {
    using namespace mpark::patterns;
    for (int i = 1; i <= 15; ++i) {
        match(i % 3, i % 5)(
            pattern(0, 0) = [] { std::cout << "fizzbuzz" << std::endl; },
            pattern(0, _) = [] { std::cout << "fiz" << std::endl; },
            pattern(_, 0) = [] { std::cout << "baz" << std::endl; },
            pattern(_, _) = [i] { std::cout << i << std::endl; });
    }
}

int factorial(int n) {
    using namespace mpark::patterns;
    return match(n)(
        pattern(0) = [] { return 1; }, pattern(_) = [n] { return n * factorial(n - 1); });
}

int main() {
    fizzbuzz();

    const auto n = 6;
    std::cout << "Factorial of " << n << " is " << factorial(n) << std::endl;
}
