#include <benchmark/benchmark.h>
#include <buffered_channel.h>

#include <thread>
#include <atomic>
#include <vector>
#include <stdexcept>

const int kCount = 5e5;

void Send(int threads_count, BufferedChannel<int>& channel) {
    auto func = [&channel, threads_count](int start) {
        for (int i = start; i < kCount; i += threads_count) {
            channel.Send(i);
        }
    };
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back(func, i);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    channel.Close();
}

int64_t CalcSum(int senders_count, int readers_count, int buff_size) {
    BufferedChannel<int> channel(buff_size);
    std::thread sender_thread(Send, senders_count, std::ref(channel));
    std::vector<std::thread> threads;
    threads.reserve(readers_count);
    std::atomic<int64_t> sum(0);
    for (int i = 0; i < readers_count; ++i) {
        threads.emplace_back([&channel, &sum]() {
            for (;;) {
                auto value = channel.Recv();
                if (!value) {
                    break;
                }
                sum += value.value();
            }
        });
    }

    for (auto& cur : threads) {
        cur.join();
    }

    sender_thread.join();

    return sum;
}

void Run(benchmark::State& state) {
    int64_t ok_ans = static_cast<int64_t>(kCount) * (kCount - 1) / 2;
    while (state.KeepRunning()) {
        int buff_size = state.range(0);
        int senders_count = state.range(1);
        int readers_count = state.range(2);
        if (ok_ans != CalcSum(senders_count, readers_count, buff_size)) {
            throw std::logic_error("Wrong sum");
        }
    }
}

const int kThreads = std::thread::hardware_concurrency();
const int kHalf = std::max(1, kThreads / 2);
const int kAnotherHalf = std::max(1, kThreads - kHalf);

BENCHMARK(Run)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond)
    ->Args({2, 2, std::max(1, kThreads - 2)})
    ->Args({10, kHalf, kAnotherHalf})
    ->Args({100000, kHalf, kAnotherHalf})
    ->MinTime(0.1);

BENCHMARK_MAIN();
