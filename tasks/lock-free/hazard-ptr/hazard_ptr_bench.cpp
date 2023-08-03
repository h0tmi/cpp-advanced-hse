#include <benchmark/benchmark.h>

#include <thread>

#include "hazard_ptr.h"

static std::atomic<int*> value = new int{42};

static void BenchmarkReadOnly(benchmark::State& state) {
    RegisterThread();

    int x = 0;
    for (auto _ : state) {
        auto ptr = Acquire(&value);
        x |= *ptr;
        Release();
    }
    benchmark::DoNotOptimize(x);

    UnregisterThread();
}

BENCHMARK(BenchmarkReadOnly)->ThreadRange(1, 16)->Unit(benchmark::kNanosecond);

static void BenchmarkRareUpdates(benchmark::State& state) {
    std::thread updater;

    std::atomic<bool> stop = false;
    if (state.thread_index == 0) {
        updater = std::thread([&] {
            RegisterThread();
            while (!stop) {
                Retire(value.exchange(new int{42}));
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            UnregisterThread();
        });
    }
    RegisterThread();

    int x = 0;
    for (auto _ : state) {
        auto ptr = Acquire(&value);
        x |= *ptr;
        Release();
    }
    benchmark::DoNotOptimize(x);

    UnregisterThread();

    if (state.thread_index == 0) {
        stop = true;
        updater.join();
    }
}

BENCHMARK(BenchmarkRareUpdates)->ThreadRange(1, 16)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
