#include <benchmark/benchmark.h>
#include <reduce.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "commons.h"

const int kMaxSize = 1000 * 1000 * 100;

uint32_t Gcd(uint32_t a, uint32_t b) {
    return !b ? a : Gcd(b, a % b);
}

const std::vector<uint32_t> kTest(GenTest<uint32_t>(kMaxSize));
const uint32_t kOkResult = std::accumulate(kTest.begin(), kTest.end(), 0u, Gcd);

void Run(benchmark::State& state) {
    while (state.KeepRunning()) {
        auto result = Reduce(kTest.begin(), kTest.end(), 0u, Gcd);
        if (result != kOkResult) {
            state.SkipWithError("Incorrect reduce result");
        }
    }
}

BENCHMARK(Run)->Unit(benchmark::kMillisecond)->UseRealTime();

BENCHMARK_MAIN();
