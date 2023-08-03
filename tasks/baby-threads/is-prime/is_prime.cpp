#include "is_prime.h"
#include <cmath>
#include <algorithm>

bool IsPrime(uint64_t x) {
    if (x <= 1) {
        return false;
    }
    uint64_t root = sqrt(x);
    auto bound = std::min(root + 6, x);
    for (auto i = 2ull; i < bound; ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}
