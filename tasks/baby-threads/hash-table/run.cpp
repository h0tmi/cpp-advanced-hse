#include <thread>
#include <vector>

#include <benchmark/benchmark.h>
#include <concurrent_hash_map.h>

#include "commons.h"

std::unique_ptr<ConcurrentHashMap<int, int>> test_table;

const int kSeed = 82944584;
auto undefined_size = ConcurrentHashMap<int, int>::kUndefinedSize;

void RandomInsertions(benchmark::State& state) {
    if (state.thread_index == 0) {
        test_table.reset(new ConcurrentHashMap<int, int>(undefined_size, state.threads));
    }

    Random random(kSeed + state.thread_index * 10);
    while (state.KeepRunning()) {
        test_table->Insert(random(), 1);
    }

    if (state.thread_index == 0) {
        test_table.reset();
    }
}

void SpecialInsertions(benchmark::State& state) {
    if (state.thread_index == 0) {
        test_table.reset(new ConcurrentHashMap<int, int>(undefined_size, state.threads));
    }

    uint32_t i = state.thread_index * 100000;
    EqualLowBits low_bits(16);
    while (state.KeepRunning()) {
        if (state.thread_index == 0) {
            test_table->Insert(low_bits(), 1);
        } else {
            test_table->Insert(i++, 1);
        }
    }

    if (state.thread_index == 0) {
        test_table.reset();
    }
}

void ManySearches(benchmark::State& state) {
    if (state.thread_index == 0) {
        test_table.reset(new ConcurrentHashMap<int, int>(undefined_size, state.threads));
        DummyLogger logger;
        MakeQueries(*test_table, logger, 100000, QueryType::INSERT, Increment(0));
    }

    Random rnd(kSeed - state.thread_index - 1, 0, 1000000);
    while (state.KeepRunning()) {
        if (state.thread_index == 0) {
            test_table->Insert(rnd(), 1);
        } else {
            test_table->Find(rnd());
        }
    }

    if (state.thread_index == 0) {
        test_table.reset();
    }
}

void Deletions(benchmark::State& state) {
    if (state.thread_index == 0) {
        test_table.reset(new ConcurrentHashMap<int, int>(undefined_size, state.threads));
    }

    Random rnd(kSeed + state.thread_index / 2);
    while (state.KeepRunning()) {
        if (state.thread_index % 2 == 0) {
            test_table->Insert(rnd(), 1);
        } else {
            test_table->Erase(rnd());
        }
    }

    if (state.thread_index == 0) {
        test_table.reset();
    }
}

BENCHMARK(RandomInsertions)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK(SpecialInsertions)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK(ManySearches)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK(Deletions)->Threads(4)->Threads(8)->UseRealTime();

BENCHMARK_MAIN();
