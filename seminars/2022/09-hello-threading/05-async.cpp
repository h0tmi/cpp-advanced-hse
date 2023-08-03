#include "timer.h"

#include <cmath>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

constexpr int kLimit = 100'000;

int main() {
    timer::Timer timer;

    std::vector<std::future<double>> futures;
    for (int i = 0; i < kLimit; ++i) {
        futures.emplace_back(std::async([i] { return std::sqrt(i); }));
    }

    double sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    std::cerr << "sum: " << sum << ", elapsed: " << timer.Elapsed() << std::endl;
}
