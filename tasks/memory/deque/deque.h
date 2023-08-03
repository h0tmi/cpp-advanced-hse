#pragma once

#include <initializer_list>
#include <algorithm>
#include <deque>

class Deque {
public:
    Deque() = default;
    Deque(const Deque& rhs) = default;
    Deque(Deque&& rhs) = default;
    explicit Deque(size_t size) : data_(size) {
    }

    Deque(std::initializer_list<int> list) : data_(list) {
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(data_, rhs.data_);
    }

    void PushBack(int value) {
        data_.push_back(value);
    }

    void PopBack() {
        data_.pop_back();
    }

    void PushFront(int value) {
        data_.push_front(value);
    }

    void PopFront() {
        data_.pop_front();
    }

    int& operator[](size_t ind) {
        return data_[ind];
    }

    int operator[](size_t ind) const {
        return data_[ind];
    }

    size_t Size() const {
        return data_.size();
    }

    void Clear() {
        data_.clear();
    }

private:
    std::deque<int> data_;
};
