#pragma once

class Any {
public:
    Any() {
    }

    template <class T>
    Any(const T& value) {
    }

    template <class T>
    Any& operator=(const T& value) {
    }

    Any(const Any& rhs) {
    }
    Any& operator=(const Any& rhs) {
    }
    ~Any() {
    }

    bool Empty() const {
        return true;
    }

    void Clear() {
    }
    void Swap(Any& rhs) {
    }

    template <class T>
    const T& GetValue() const {
    }
};
