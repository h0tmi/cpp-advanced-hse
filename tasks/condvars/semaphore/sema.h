#pragma once

#include <list>
#include <queue>
#include <functional>
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
        if (!queue_.empty()) {
            queue_.front()();
            queue_.pop();
        } else {
            ++count_;
        }
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!count_) {
            std::function<void()> wrapper = [this, &callback]() {
                callback(count_);
            };
            queue_.push(wrapper);
            cv_.wait(lock);
        } else {
            callback(count_);
        }
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_ = 0;
    std::queue<std::function<void()>> queue_;
};
