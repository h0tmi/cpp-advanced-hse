#pragma once

template <class T>
class ImmutableVector {
public:
    ImmutableVector() {
    }

    explicit ImmutableVector(size_t count, const T& value = T()) {
    }

    template <typename Iterator>
    ImmutableVector(Iterator first, Iterator last) {
    }

    ImmutableVector(std::initializer_list<T> l) {
    }

    ImmutableVector Set(size_t index, const T& value) {
    }

    const T& Get(size_t index) const {
        static T dummy_value;
        return dummy_value;
    }

    ImmutableVector PushBack(const T& value) {
    }

    ImmutableVector PopBack() {
    }

    size_t Size() const {
        return 0;
    }
};
