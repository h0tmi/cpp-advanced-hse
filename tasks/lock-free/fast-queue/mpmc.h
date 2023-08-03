#pragma once

#include <mutex>
#include <queue>

template <class T>
class MPMCBoundedQueue {
public:
    explicit MPMCBoundedQueue(int size) {
        max_size_ = size;
    }

    bool Enqueue(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() == max_size_) {
            return false;
        }
        queue_.push(value);
        return true;
    }

    bool Dequeue(T& data) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        data = std::move(queue_.front());
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    int max_size_ = 0;
};
