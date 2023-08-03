#include "timer.h"

#include <cstddef>
#include <memory>
#include <thread>
#include <atomic>
#include <cstdint>
#include <iostream>

class RacyCounter {
public:
    void Add(std::uint64_t delta) {
        value_ = value_ + delta;
    }

    std::uint64_t Get() const {
        return value_;
    }

private:
    volatile std::uint64_t value_ = 0;
};

class AtomicCounter {
public:
    void Add(std::uint64_t delta) {
        value_.fetch_add(delta);
    }

    std::uint64_t Get() const {
        return value_.load();
    }

private:
    std::atomic<std::uint64_t> value_ = 0;
};

template <typename Accumulator>
void Summator(Accumulator& acc, std::uint64_t lo, std::uint64_t hi) {
    while (lo < hi) {
        acc.Add(lo++);
    }
}

constexpr size_t kLimit = 100'000'000;

void RacySum() {
    timer::Timer timer;
    RacyCounter sum;
    {
        std::jthread th1{Summator<RacyCounter>, std::ref(sum), 0, kLimit / 2};
        std::jthread th2{Summator<RacyCounter>, std::ref(sum), kLimit / 2, kLimit};
    }
    std::cout << "sum: " << sum.Get() << ", expected: " << (kLimit * (kLimit - 1)) / 2
              << ", elapsed: " << timer.Elapsed() << std::endl;
}

void AtomicSum() {
    timer::Timer timer;
    AtomicCounter sum;
    {
        std::jthread th1{Summator<AtomicCounter>, std::ref(sum), 0, kLimit / 2};
        std::jthread th2{Summator<AtomicCounter>, std::ref(sum), kLimit / 2, kLimit};
    }
    std::cout << "sum: " << sum.Get() << ", expected: " << (kLimit * (kLimit - 1)) / 2
              << ", elapsed: " << timer.Elapsed() << std::endl;
}

int main() {
    RacySum();
    AtomicSum();
}
