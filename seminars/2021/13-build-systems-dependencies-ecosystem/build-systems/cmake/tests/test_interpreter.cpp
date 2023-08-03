#include <racket/interpreter.hpp>

#include <cassert>
#include <iostream>

auto main() -> int {
    auto interpreter = racket::Interpreter{};

    { assert(interpreter.Run("Expression") == ""); }

    std::cout << "Tests passed" << std::endl;
}
