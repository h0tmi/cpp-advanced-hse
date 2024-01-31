#pragma once

#include <vector>
#include <iterator>
#include <thread>

template <class RandomAccessIterator, class T, class Func>
void ReduceThread(RandomAccessIterator first, RandomAccessIterator last, const T initial_value,
                  Func func, int64_t* result) {
    auto cur_value(initial_value);
    while (first != last) {
        cur_value = func(cur_value, *first++);
    }
    *result = cur_value;
}

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    size_t n = std::distance(first, last);
    size_t thread_num = std::thread::hardware_concurrency();
    std::vector<int64_t> values(thread_num + 1, 0);
    thread_num = std::max(n / thread_num, static_cast<size_t>(1));
    std::vector<std::thread> workers;
    size_t current_value = 0;

    for (size_t i = 0; i < n; i += thread_num) {
        if (i + thread_num < n) {
            workers.emplace_back(ReduceThread<RandomAccessIterator, T, Func>, first,
                                 first + thread_num, initial_value, func, &values[current_value++]);
            first = first + thread_num;
        } else {
            workers.emplace_back(ReduceThread<RandomAccessIterator, T, Func>, first, last,
                                 initial_value, func, &values[current_value++]);
        }
    }
    for (auto& worker : workers) {
        worker.join();
    }
    auto cur_value(initial_value);
    for (size_t i = 0; i < current_value; ++i) {
        cur_value = func(values[i], cur_value);
    }
    return cur_value;
}
