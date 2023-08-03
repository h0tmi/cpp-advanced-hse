#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>
#include <deque>

class Latch {
public:
    Latch(int initial_value) : value_{initial_value} {
    }

    void Arrive() {
        std::lock_guard lock{mtx_};
        --value_;
        if (value_ == 0) {
            cv_.notify_all();
        }
    }

    void Wait() {
        std::unique_lock lock{mtx_};
        cv_.wait(lock, [this] { return value_ == 0; });
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    int value_ = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Bucket {
    Bucket(size_t num_threads) : sum_by_thread(num_threads, 0.0), latch(num_threads) {
    }

    std::vector<double> sum_by_thread;
    Latch latch;
};

struct State {
    State(size_t num_threads) : num_threads{num_threads} {
        size_t num_buckets = limit / bucket_size;
        for (size_t i = 0; i < num_buckets; ++i) {
            buckets.emplace_back(num_threads);
        }
    }

    size_t limit = 1'000'000'000;
    size_t num_threads = 0;
    size_t bucket_size = 100'000'000;
    std::deque<Bucket> buckets;
};

void Worker(State& state, size_t thread_index) {
    size_t prev_bucket = 0;
    for (size_t num = thread_index; num < state.limit; num += state.num_threads) {
        size_t bucket = num / state.bucket_size;
        if (bucket > prev_bucket) {
            state.buckets[prev_bucket].latch.Arrive();
            prev_bucket = bucket;
        }
        state.buckets[bucket].sum_by_thread[thread_index] += std::sqrt(num);
    }
    state.buckets[prev_bucket].latch.Arrive();
}

int main() {
    State state{std::thread::hardware_concurrency()};
    std::vector<std::jthread> workers;
    for (size_t i = 0; i < state.num_threads; ++i) {
        workers.emplace_back(Worker, std::ref(state), i);
    }

    double sum = 0.0;
    for (auto&& bucket : state.buckets) {
        bucket.latch.Wait();
        double delta =
            std::accumulate(bucket.sum_by_thread.begin(), bucket.sum_by_thread.end(), 0.0);
        sum += delta;
        std::cout << "Current sum: " << sum << ", delta: " << delta << std::endl;
    }
}
