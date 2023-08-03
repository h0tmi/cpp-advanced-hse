#include "chromo/time.hpp"

#include <iostream>

auto main() -> int {
    std::cout << "Current time is " << chromo::Now().since_unix_epoch << std::endl;
}
