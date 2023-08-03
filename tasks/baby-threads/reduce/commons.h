#pragma once

#include <vector>
#include <random>

template <class T>
class Summator {
public:
    T operator()(const T& sum, const T& cur_value) const {
        return sum + cur_value;
    }
};

template <class T>
class Multiplier {
public:
    T operator()(const T& mul, const T& cur_value) const {
        return mul * cur_value;
    }
};

template <class T>
std::vector<T> GenTest(int size) {
    std::vector<T> result(size);
    static std::mt19937 gen(7347475);
    for (int i = 0; i < size; ++i) {
        result[i] = gen();
    }
    return result;
}
