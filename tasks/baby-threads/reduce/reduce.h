#pragma once

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    auto cur_value(initial_value);
    while (first != last) {
        cur_value = func(cur_value, *first++);
    }
    return cur_value;
}
