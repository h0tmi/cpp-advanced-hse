#include "deep_thought.hpp"

#include <iostream>

auto main() -> int {
    const auto deep_thought = DeepThought{};
    std::cout << "The ultimate answer is " << deep_thought.UltimateAnswer() << std::endl;
}
