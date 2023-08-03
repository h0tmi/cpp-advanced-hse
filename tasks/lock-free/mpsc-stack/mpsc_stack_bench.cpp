#include <mpsc_stack.h>

#include <benchmark/benchmark.h>

#include <thread>

static void BenchmarkPushPop(benchmark::State& state) {
    MPSCStack<int> stack;

    for (auto _ : state) {
        stack.Push(42);
        stack.Pop();
    }
}

BENCHMARK(BenchmarkPushPop)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
