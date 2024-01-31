#pragma once

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <set>
#include <system_error>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

public:
    void Add(const T& item, TimePoint at) {
        std::unique_lock guard(mutex_);
        queue_.emplace(at, item);
        cond_var_.notify_one();
    }

    T Pop() {
        std::unique_lock guard(mutex_);
        while (queue_.empty()) {
            cond_var_.wait(guard);
        }

        cond_var_.wait_until(guard, queue_.begin()->first, [this]() {
            return std::chrono::system_clock::now() >= queue_.begin()->first;
        });

        auto it = queue_.begin();
        auto value = it->second;

        queue_.erase(it);

        return value;
    }

private:
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::set<std::pair<std::chrono::system_clock::time_point, T>> queue_;
};
