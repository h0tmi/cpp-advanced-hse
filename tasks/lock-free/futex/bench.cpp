#include <benchmark/benchmark.h>

#include "mutex.h"
#include <memory>

int counter;
std::shared_ptr<Mutex> mutex;

void Run(benchmark::State& state) {
    if (state.thread_index == 0) {
        counter = 0;
        mutex = std::make_shared<Mutex>();
    }
    while (state.KeepRunning()) {
        mutex->Lock();
        ++counter;
        mutex->Unlock();
    }
}

BENCHMARK(Run)->UseRealTime()->ThreadRange(1, 8);

BENCHMARK_MAIN();
