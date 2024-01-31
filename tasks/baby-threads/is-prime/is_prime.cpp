#include "is_prime.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <iostream>
#include <vector>
#include <thread>

void IsPrimeThread(uint64_t l, uint64_t r, uint64_t x, std::atomic<bool>* is_prime) {
    if (!is_prime->load()) {
        return;
    }
    for (auto i = l; i <= r; ++i) {
        if (x % i == 0) {
            is_prime->store(false);
        }
    }
}

bool IsPrime(uint64_t x) {
    if (x <= 1 || (x > 2 && x % 2 == 0)) {
        return false;
    }

    size_t thread_num = std::thread::hardware_concurrency();
    uint64_t root = sqrt(x);
    size_t bound = std::min(root + 6, x);
    size_t step = std::max(bound / thread_num, static_cast<size_t>(1));
    std::atomic<bool> is_prime;
    is_prime.store(true);
    std::vector<std::thread> workers;
    for (auto i = 2ull; i < bound; i += step) {
        if (i + step < bound) {
            workers.emplace_back(IsPrimeThread, i, i + step, x, &is_prime);
        } else {
            workers.emplace_back(IsPrimeThread, i, bound, x, &is_prime);
        }
    }
    for (std::thread& t : workers) {
        t.join();
    }
    return is_prime;
}
