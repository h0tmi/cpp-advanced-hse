#include <iostream>

template <class T>
class Foo {
public:
    Foo(T&& lambda) {
        // ...
    }
};

auto main() -> int {
    auto lambda = [] { std::cout << "Lambda!" << '\n'; };
    Foo f(std::move(lambda));

    // What for?
    // https://github.com/ClickHouse/ClickHouse/blob/b097140bb40b72de107e296f9bf4ac4d42a1c94e/src/DataStreams/copyData.cpp#L21

    // Templates instantiate functions
    // for every different type
    //   => final binary possibly has multiple
    //      copies of the same function but
    //      for different types
    //   => multiple templates with multiple
    //      instantiations will bloat binary
    //   => huge binary means more cache misses
    //   => slower performance in some cases
}
