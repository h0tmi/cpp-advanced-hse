#include <racket/interpreter.hpp>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

using Clock = std::chrono::system_clock;

#define ASSERT(condition)                                                                \
    do {                                                                                 \
        if (!(condition)) {                                                              \
            throw std::runtime_error{std::string{"Assert `"} + #condition + "` failed"}; \
        }                                                                                \
    } while (false)

auto main() -> int {
    auto interpreter = racket::Interpreter{};

    {
        const auto begin = Clock::now();

        interpreter.Run("Expression");

        const auto end = Clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        ASSERT(duration.count() <= 100);
    }

    std::cout << "Benchmarks passed" << std::endl;
}
