#pragma once

#include <mutex>
#include <condition_variable>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    Semaphore(int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!count_) {
            cv_.wait(lock);
        }
        callback(count_);
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_ = 0;
};
