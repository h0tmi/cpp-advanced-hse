#pragma once

#include <utility>
#include <optional>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) {
        // Your code goes here
    }

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
