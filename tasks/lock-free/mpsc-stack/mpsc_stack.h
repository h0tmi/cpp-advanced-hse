#pragma once

#include <atomic>
#include <optional>
#include <stdexcept>
#include <utility>

template <class T>
class MPSCStack {
public:
    // Push adds one element to stack top.
    //
    // Safe to call from multiple threads.
    void Push(const T& value) {
        (void)value;
        throw std::runtime_error{"Unimplemented"};
    }

    // Pop removes top element from the stack.
    //
    // Not safe to call concurrently.
    std::optional<T> Pop() {
        throw std::runtime_error{"Unimplemented"};
    }

    // DequeuedAll Pop's all elements from the stack and calls cb() for each.
    //
    // Not safe to call concurrently with Pop()
    template <class TFn>
    void DequeueAll(const TFn& cb) {
        (void)cb;
        throw std::runtime_error{"Unimplemented"};
    }

    ~MPSCStack() {
    }
};
