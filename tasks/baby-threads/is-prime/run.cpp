#include <cstdint>
#include <array>
#include <benchmark/benchmark.h>
#include "is_prime.h"

const std::array<uint64_t, 4> kNumbers = {1000000000000000177ull, 1000000000375847551ull,
                                          3778118040573702001ull,
                                          1000ull * 1000 * 1000 * 1000 * 1000 * 1000};

const std::array<bool, 4> kResults = {true, true, false, false};

void CheckNumbers(benchmark::State& state) {
    while (state.KeepRunning()) {
        uint64_t num = kNumbers[state.range(0)];
        if (IsPrime(num) != kResults[state.range(0)]) {
            state.SkipWithError("Invalid prime check");
        }
    }
}

BENCHMARK(CheckNumbers)
    ->DenseRange(0, static_cast<int>(kNumbers.size()) - 1)
    ->Unit(benchmark::kMicrosecond)
    ->UseRealTime();

BENCHMARK_MAIN();
