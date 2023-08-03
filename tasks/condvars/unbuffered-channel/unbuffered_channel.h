#pragma once

#include <utility>
#include <optional>

template <class T>
class UnbufferedChannel {
public:
    void Send(const T& value) {
        // Your code goes here
    }

    std::optional<T> Recv() {
        // Your code goes here
        return std::nullopt;
    }

    void Close() {
        // Your code goes here
    }
};
