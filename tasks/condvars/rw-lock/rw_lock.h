#pragma once
#include <mutex>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        read_.lock();
        ++blocked_readers_;
        if (blocked_readers_ == 1) {
            global_.lock();
        }
        read_.unlock();
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
        std::lock_guard<std::mutex> lock(global_);
        func();
    }

private:
    std::mutex read_;
    std::mutex global_;
    int blocked_readers_ = 0;

    void EndRead() {
        read_.lock();
        --blocked_readers_;
        if (!blocked_readers_) {
            global_.unlock();
        }
        read_.unlock();
    }
};
