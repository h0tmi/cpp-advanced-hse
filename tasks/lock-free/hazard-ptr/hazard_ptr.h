#pragma once

#include <atomic>
#include <functional>
#include <shared_mutex>
#include <mutex>
#include <memory>

extern std::shared_mutex lock;

template <class T>
T* Acquire(std::atomic<T*>* ptr) {
    lock.lock_shared();
    return ptr->load();
}

inline void Release() {
    lock.unlock_shared();
}

template <class T, class Deleter = std::default_delete<T>>
void Retire(T* value, Deleter deleter = {}) {
    std::unique_lock<std::shared_mutex> guard(lock);
    deleter(value);
}

inline void RegisterThread() {
}

inline void UnregisterThread() {
}
