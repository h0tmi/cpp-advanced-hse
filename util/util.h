#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

class RandomGenerator {
public:
    explicit RandomGenerator(uint32_t seed = 738547485u) : gen_(seed) {
    }

    template <class T>
    std::vector<T> GenIntegralVector(size_t count, T from, T to) {
        std::uniform_int_distribution<T> dist(from, to);
        std::vector<T> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::string GenString(size_t count, char from = 'a', char to = 'z') {
        std::uniform_int_distribution<int> dist(from, to);
        std::string result(count, from);
        for (char& x : result) {
            x = dist(gen_);
        }
        return result;
    }

    std::vector<double> GenRealVector(size_t count, double from, double to) {
        std::uniform_real_distribution<double> dist(from, to);
        std::vector<double> result(count);
        for (auto& cur : result) {
            cur = dist(gen_);
        }
        return result;
    }

    std::vector<int> GenPermutation(size_t count) {
        std::vector<int> result(count);
        std::iota(result.begin(), result.end(), 0);
        std::shuffle(result.begin(), result.end(), gen_);
        return result;
    }

    template <class T>
    T GenInt(T from, T to) {
        std::uniform_int_distribution<T> dist(from, to);
        return dist(gen_);
    }

    template <class T>
    T GenInt() {
        return GenInt(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    template <class Iterator>
    void Shuffle(Iterator first, Iterator last) {
        std::shuffle(first, last, gen_);
    }

private:
    std::mt19937 gen_;
};
