#include "timer.h"
#include "queue.h"

#include <cmath>
#include <iostream>
#include <optional>
#include <queue>
#include <thread>

constexpr int kLimit = 100'000;

void SlowSum() {
    timer::Timer timer;

    double sum = 0;
    for (int i = 0; i < kLimit; ++i) {
        std::thread t{[i, &sum] { sum += std::sqrt(i); }};
        t.join();
        if (i % 10000 == 0) {
            std::cerr << "iteration #" << i << std::endl;
        }
    }
    std::cerr << "sum: " << sum << ", elapsed: " << timer.Elapsed() << std::endl;
}

void FastSum() {
    timer::Timer timer;

    double sum = 0;
    UnboundedBlockingQueue<int> queue;
    std::thread t{[&queue, &sum] {
        while (auto value = queue.Take()) {
            sum += std::sqrt(*value);
        }
    }};
    for (int i = 0; i < kLimit; ++i) {
        queue.Put(i);
        if (i % 10000 == 0) {
            std::cerr << "iteration #" << i << std::endl;
        }
    }
    queue.Close();
    t.join();
    std::cerr << "sum: " << sum << ", elapsed: " << timer.Elapsed() << std::endl;
}

int main() {
    SlowSum();
    FastSum();
}
