#include <benchmark/benchmark.h>
#include <atomic>
#include <stdexcept>

#include <rw_spinlock.h>

RWSpinLock lock;
std::atomic<int> counter = {0};

void Reads(benchmark::State& state) {
    while (state.KeepRunning()) {
        lock.LockRead();
        lock.UnlockRead();
    }
}

void Writes(benchmark::State& state) {
    while (state.KeepRunning()) {
        lock.LockWrite();
        if (counter++) {
            throw std::runtime_error("invalid counter");
        }
        counter--;
        lock.UnlockWrite();
    }
}

void ReadsWrites(benchmark::State& state) {
    if (state.thread_index % 2) {
        while (state.KeepRunning()) {
            lock.LockRead();
            lock.UnlockRead();
        }
    } else {
        while (state.KeepRunning()) {
            lock.LockWrite();
            if (counter++) {
                throw std::runtime_error("invalid counter");
            }
            counter--;
            lock.UnlockWrite();
        }
    }
}

BENCHMARK(Reads)->ThreadRange(1, 8)->UseRealTime();
BENCHMARK(Writes)->ThreadRange(1, 8)->UseRealTime();
BENCHMARK(ReadsWrites)->ThreadRange(1, 8)->UseRealTime();

BENCHMARK_MAIN();
