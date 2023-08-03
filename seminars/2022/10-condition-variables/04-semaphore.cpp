#include "burn.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class Semaphore {
public:
    Semaphore(int initial) : counter_{initial} {
    }

    // NOLINTNEXTLINE
    void lock() {
        std::unique_lock lock{mutex_};
        not_empty_.wait(lock, [this] { return counter_ > 0; });
        --counter_;
    }

    // NOLINTNEXTLINE
    void unlock() {
        {
            std::unique_lock lock{mutex_};
            ++counter_;
        }

        // Unlock mutex here!
        not_empty_.notify_one();
    }

private:
    int counter_ = 0;
    std::mutex mutex_;
    std::condition_variable not_empty_;
};

int main() {
    Semaphore semaphore{10};

    std::vector<std::jthread> threads;
    for (int i = 0; i < 1000; ++i) {
        threads.emplace_back([&semaphore] {
            std::lock_guard lock{semaphore};
            BurnCpuFor(std::chrono::milliseconds(100));
        });
    }
}
