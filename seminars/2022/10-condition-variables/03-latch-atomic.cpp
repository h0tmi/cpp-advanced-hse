#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

class Latch {
public:
    Latch(int initial_value) : value_{initial_value} {
    }

    void Arrive() {
        if (value_.fetch_sub(1) == 1) {
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
    std::atomic<int> value_ = 0;
};
