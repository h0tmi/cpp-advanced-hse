#pragma once

#include <type_traits>
#include <vector>

struct Sum {
    int operator()(int a, int b) {
        return a + b;
    }
};

struct Prod {
    int operator()(int a, int b) {
        return a * b;
    }
};

struct Concat {
    std::vector<int> operator()(const std::vector<int>& a, const std::vector<int>& b) {
        std::vector<int> res;
        res = a;
        res.insert(res.end(), b.begin(), b.end());
        return res;
    }
};

class Length {
public:
    Length(int* size) : size_(size){};
    template <typename T>
    T operator()([[maybe_unused]] const T& a, [[maybe_unused]] const T& b) {
        ++(*size_);
        return a;
    }

private:
    int* size_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    if (first == last) {
        return init;
    }
    Iterator pos = first;
    ++pos;
    return func(init, Fold(pos, last, *first, func));
}
