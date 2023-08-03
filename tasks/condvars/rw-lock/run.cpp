#include "rw_lock.h"

#include <benchmark/benchmark.h>

#include <vector>
#include <thread>
#include <stdexcept>

int64_t MulMod(int64_t a, int64_t b, int64_t modulo) {
    int64_t res = 0;
    while (a > 0) {
        if (a & 1) {
            res += b;
            if (res >= modulo) {
                res -= modulo;
            }
        }
        a >>= 1;
        b <<= 1;
        if (b >= modulo) {
            b -= modulo;
        }
    }
    return res;
}

int64_t BinPow(int64_t a, int64_t b, int64_t c) {
    if (!b) {
        return 1;
    } else if (b % 2 == 0) {
        int64_t result = BinPow(a, b / 2, c);
        return MulMod(result, result, c);
    } else {
        return MulMod(a, BinPow(a, b - 1, c), c);
    }
}

std::unique_ptr<RWLock> rw_lock;
std::unique_ptr<int64_t> counter;

void RunBench(benchmark::State& state, int read_count) {
    if (state.thread_index == 0) {
        rw_lock.reset(new RWLock());
        counter.reset(new int64_t(0));
    }
    const int64_t prime = 100000000000000003ll;
    while (state.KeepRunning()) {
        if (state.thread_index < read_count) {
            rw_lock->Read([]() {
                if (*counter > 0 && BinPow(*counter, prime - 1, prime) != 1) {
                    throw std::logic_error("Fermat's little theorem is wrong?");
                }
            });
        } else {
            rw_lock->Write([]() { ++(*counter); });
        }
    }
}

void Half(benchmark::State& state) {
    RunBench(state, state.threads / 2);
}

void ReadOnly(benchmark::State& state) {
    RunBench(state, state.threads);
}

void Reads(benchmark::State& state) {
    RunBench(state, std::max(2, state.threads - 2));
}

const int kThreads = std::thread::hardware_concurrency();

BENCHMARK(Half)->Threads(kThreads)->UseRealTime();
BENCHMARK(ReadOnly)->Threads(kThreads)->UseRealTime();
BENCHMARK(Reads)->Threads(kThreads)->UseRealTime();

BENCHMARK_MAIN();
