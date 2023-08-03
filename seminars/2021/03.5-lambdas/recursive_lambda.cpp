#include <iostream>

auto main() -> int {
    // In C++ it is possible to do this:
    int foo = foo;
    std::cout << foo << std::endl;

    // Why? Standard:
    // "The point of declaration for a name
    //  is immediately after its complete declarator
    //  and before its initializer (if any)..."

    int bar     // Bar is declared now (`int`)
        = bar;  // and initialized with its own value.

    // Same happens below.
    int zoo;
    zoo = zoo;

    auto sum = [](auto left, auto right) {
        // Cannot use `sum` here because of the `auto` type :(
        // return 1 + sum(left - 1, right);

        // Add indirection
        auto impl = [](auto left, auto right, auto& impl_ref) {
            if (left < 0) {
                return 0;
            } else {
                return left + impl_ref(left - 1, right, impl_ref);
            }
        };
        return impl(left, right, impl);
    };
    std::cout << sum(5, 7) << std::endl;

    // `std::function` has determined type
    // before `=` and allows recursive calls,
    //
    // BUT `std::function` may allocate in
    // order to store large objects inside.
    std::function<int(int)> factorial = [&](int n) {
        if (n <= 0) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    };
    std::cout << factorial(5) << std::endl;
}
