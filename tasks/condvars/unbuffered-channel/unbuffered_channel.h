// #pragma once
//
// #include <utility>
// #include <optional>
//
// template <class T>
// class UnbufferedChannel {
// public:
//     void Send(const T& value) {
//         // Your code goes here
//     }
//
//     std::optional<T> Recv() {
//         // Your code goes here
//         return std::nullopt;
//     }
//
//     void Close() {
//         // Your code goes here
//     }
// };
//
#pragma once

#include <utility>
#include <optional>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <class T>
class UnbufferedChannel {
private:
    int size_;
    std::optional<T> data_;
    std::atomic<bool> closed_ = false;
    std::atomic<bool> new_elem_ = false;
    mutable std::mutex global_send_;
    mutable std::mutex global_rec_;
    std::mutex mutex_;
    std::condition_variable spare_place_;
    std::condition_variable added_elem_;

public:
    void Send(const T& value) {
        std::lock_guard<std::mutex> lock_guard(global_send_);
        std::unique_lock<std::mutex> lock(mutex_);
        data_ = value;
        new_elem_ = true;
        added_elem_.notify_one();
        while (!closed_ && new_elem_) {
            spare_place_.wait(lock);
        }
        if (closed_ && data_.has_value()) {
            throw std::runtime_error("Channel is closed");
        }
    }

    std::optional<T> Recv() {
        std::lock_guard<std::mutex> lock_guard(global_rec_);
        std::unique_lock<std::mutex> lock(mutex_);
        while (!closed_ && !new_elem_) {
            added_elem_.wait(lock);
        }
        if (closed_) {
            return std::nullopt;
        }
        std::optional<T> res = data_;
        new_elem_ = false;
        data_ = std::nullopt;
        spare_place_.notify_one();
        return res;
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_ = true;
        new_elem_ = false;
        spare_place_.notify_one();
        added_elem_.notify_one();
    }
};
