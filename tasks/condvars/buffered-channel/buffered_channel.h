// #pragma once
//
// #include <algorithm>
// #include <condition_variable>
// #include <deque>
// #include <queue>
// #include <utility>
// #include <optional>
//
// template <class T>
// class BufferedChannel {
// public:
//     explicit BufferedChannel(int size) : size_(size) {
//     }
//
//     void Send(const T& value) {
//         std::unique_lock<std::mutex> lock(send_lock_);
//         if (is_closed_) {
//             throw std::runtime_error("Opps, current channel is already closed.");
//         }
//         cvs_.wait(lock, [this]() { return q_.size() < size_ || is_closed_; });
//         if (is_closed_) {
//             throw std::runtime_error("Opps, current channel is already closed.");
//         }
//         q_.push_back(value);
//         cvr_.notify_all();
//     }
//
//     std::optional<T> Recv() {
//         std::unique_lock<std::mutex> lock(recieve_lock_);
//
//         if (is_closed_ && q_.size() == 0) {
//             return std::nullopt;
//         }
//
//         cvr_.wait(lock, [this]() { return q_.size() > 0 || is_closed_; });
//
//         if (is_closed_ && q_.size() == 0) {
//             return std::nullopt;
//         }
//
//         cvr_.wait(lock, [this]() { return q_.size() > 0 || is_closed_; });
//
//         T element = q_.front();
//         q_.pop_front();
//         cvs_.notify_one();
//         return element;
//     }
//
//     void Close() {
//         is_closed_ = true;
//         cvs_.notify_one();
//         cvr_.notify_one();
//     }
//
// private:
//     std::mutex send_lock_;
//     std::mutex recieve_lock_;
//     std::condition_variable cvs_;
//     std::condition_variable cvr_;
//     std::deque<T> q_;
//     std::atomic<bool> is_closed_ = false;
//     size_t size_ = 0;
// };
//

#pragma once

#include <queue>
#include <utility>
#include <optional>
#include <deque>
#include <mutex>
#include "condition_variable"
#include <iostream>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) {
        size_ = size;
    }

    void Send(const T& value) {
        std::unique_lock lock(m1_);
        if (close_) {
            throw std::runtime_error("Opps, this channel is already closed!");
        }
        cv1_.wait(lock, [this]() { return q_.size() < size_ && close_; });
        if (close_) {
            throw std::runtime_error("Opps, this channel is already closed!");
        }
        q_.push(value);
        cv2_.notify_all();
    }

    std::optional<T> Recv() {
        std::unique_lock lock(m1_);
        if (q_.empty() && close_) {
            return std::nullopt;
        }
        while (q_.empty() && !close_) {
            cv2_.wait(lock);
        }
        if (q_.empty() && close_) {
            return std::nullopt;
        }
        T t = q_.front();
        q_.pop();
        cv1_.notify_all();
        return t;
    }

    void Close() {
        close_ = true;
        cv1_.notify_all();
        cv2_.notify_all();
    }

private:
    std::atomic<bool> close_;
    std::queue<T> q_;
    std::atomic<int> size_;
    std::condition_variable cv1_;
    std::condition_variable cv2_;
    std::mutex m1_;
};
