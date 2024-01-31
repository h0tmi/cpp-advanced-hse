#pragma once
#include <mutex>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        while (is_writing_) {
            cv_.wait(lock);
        }
        ++blocked_readers_;
        lock.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        while (blocked_readers_) {
            cv_.wait(lock);
        }
        while(is_writing_) {
            cv_.wait(lock);
        }
        is_writing_ = true;
        func();
        is_writing_ = false;
        cv_.notify_one();
    }

private:
    std::mutex global_;
    bool is_writing_;
    std::condition_variable cv_;
    std::atomic<int> blocked_readers_ = 0;

    void EndRead() {
        std::lock_guard<std::mutex> lock(global_);
        --blocked_readers_;
        if (!blocked_readers_) {
            cv_.notify_one();
        }
    }
};
