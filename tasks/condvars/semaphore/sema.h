#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

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
        cv_list_.front().notify_one();
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_list_.emplace();
        while (!count_) {
            cv_list_.back().wait(lock);
        }
        callback(count_);
        cv_list_.pop();
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::queue<std::condition_variable> cv_list_;
    int count_ = 0;
};
