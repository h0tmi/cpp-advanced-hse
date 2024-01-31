#include <benchmark/benchmark.h>
#include <util.h>
#include <vector>
#include <stdexcept>
#include "commons.h"
#include "find_subsets.h"

const int kMaxCount = 30;

void NoAnswer(benchmark::State& state) {
    RandomGenerator gen(735745);
    std::vector<int64_t> data{1};
    for (int i = 1; i < kMaxCount; ++i) {
        data.push_back(data.back() * 2);
    }
    while (state.KeepRunning()) {
        gen.Shuffle(data.begin(), data.end());
        auto result = FindEqualSumSubsets(data);
        if (result.exists) {
            throw std::logic_error("Non existing answer has been found");
        }
    }
}

void AnswerExists(benchmark::State& state) {
    RandomGenerator gen(1234556);
    auto data = FillData(gen, kMaxCount, 10, 10);
    while (state.KeepRunning()) {
        gen.Shuffle(data.begin(), data.end());
        auto result = FindEqualSumSubsets(data);
        if (!result.exists) {
            throw std::logic_error("Answer hasn't been found");
        }
    }
}

BENCHMARK(NoAnswer)->UseRealTime()->Unit(benchmark::kMillisecond);
BENCHMARK(AnswerExists)->UseRealTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
