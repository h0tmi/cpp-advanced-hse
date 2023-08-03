#include "generator.h"

#include <iostream>


generator<int> range(int max) {
    for (int n = 0; n < max; ++n) {
        co_yield n;
    }
}

int main() {
    auto gen = range(10);
    while (gen.next()) {
        std::cout << gen.value() << std::endl;
    }
}
