#include <benchmark/benchmark.h>
#include <atomic>
#include <array>
#include <vector>
#include <stdexcept>

#include <mpmc.h>

std::array<std::atomic<int>, 32> enqueued;
std::array<std::atomic<int>, 32> dequeued;
std::atomic<int> counter;

MPMCBoundedQueue<int> bounded_queue(64);

void StressEnqueue(benchmark::State& state) {
    while (state.KeepRunning()) {
        bounded_queue.Enqueue(0);
    }
}

void StressEnqueueDequeue(benchmark::State& state) {
    if (state.thread_index % 2) {
        while (state.KeepRunning()) {
            bounded_queue.Enqueue(0);
        }
    } else {
        int val;
        while (state.KeepRunning()) {
            bounded_queue.Dequeue(val);
        }
    }
}

void CorrectnessEnqueueDequeue(benchmark::State& state) {
    ++counter;
    if (state.thread_index == 0) {
        int val;
        while (bounded_queue.Dequeue(val)) {
            bounded_queue.Dequeue(val);
        }
    }
    if (state.thread_index % 2) {
        int index = state.thread_index / 2;
        int counter = 0;
        while (state.KeepRunning()) {
            if (bounded_queue.Enqueue(index)) {
                ++counter;
            }
        }
        enqueued[index] += counter;
    } else {
        int val;
        std::vector<int> counters(32, 0);
        while (state.KeepRunning()) {
            if (bounded_queue.Dequeue(val)) {
                ++counters[val];
            }
        }
        for (size_t i = 0; i < 32; ++i) {
            dequeued[i] += counters[i];
        }
    }
    if (--counter == 0) {
        int val;
        while (bounded_queue.Dequeue(val)) {
            ++dequeued[val];
        }
        for (size_t i = 0; i < enqueued.size(); ++i) {
            if (enqueued[i] != dequeued[i]) {
                throw std::runtime_error("invalid enqueued/dequeued counters");
            }
        }
    }
}

BENCHMARK(StressEnqueue)->ThreadRange(1, 8)->MeasureProcessCPUTime()->UseRealTime();
BENCHMARK(StressEnqueueDequeue)->ThreadRange(1, 8)->MeasureProcessCPUTime()->UseRealTime();
BENCHMARK(CorrectnessEnqueueDequeue)->ThreadRange(1, 8)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK_MAIN();
