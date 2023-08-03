#include <benchmark/benchmark.h>

#include "sync_map.h"

#include <shared_mutex>
#include <unordered_map>

static std::unordered_map<int, int> shared_map;
static std::shared_mutex shared_mutex;

static void BenchmarkSharedMutex(benchmark::State& state) {
    if (state.thread_index == 0) {
        for (int i = 0; i < 1024; i++) {
            shared_map.emplace(i, i);
        }
    }

    for (auto _ : state) {
        std::shared_lock<std::shared_mutex> guard(shared_mutex);
        auto it = shared_map.find(state.thread_index);
        if (it == shared_map.end()) {
            state.SkipWithError("index missing in map");
        }

        benchmark::DoNotOptimize(it->second);
    }
}

BENCHMARK(BenchmarkSharedMutex)->DenseThreadRange(1, 16);

static SyncMap<int, int> sync_map;

static void BenchmarkSyncMap(benchmark::State& state) {
    RegisterThread();
    if (state.thread_index == 0) {
        for (int i = 0; i < 1024; i++) {
            sync_map.Insert(i, i);
        }
    }

    for (auto _ : state) {
        int value;
        bool ok = sync_map.Lookup(state.thread_index, &value);
        if (!ok) {
            state.SkipWithError("index missing in map");
        }

        benchmark::DoNotOptimize(value);
    }
    UnregisterThread();
}

BENCHMARK(BenchmarkSyncMap)->DenseThreadRange(1, 16);

BENCHMARK_MAIN();
