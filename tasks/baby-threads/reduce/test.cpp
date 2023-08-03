#include <gtest/gtest.h>
#include <reduce.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include "commons.h"

TEST(Correctness, Empty) {
    std::vector<int> values{1, 2, 3};
    ASSERT_EQ(6,
              Reduce(values.begin(), values.end(), 0, [](int sum, int cur) { return sum + cur; }));

    std::vector<int> empty;
    ASSERT_EQ(0, Reduce(empty.begin(), empty.end(), 0, Summator<int>()));

    std::vector<int> one{1};
    ASSERT_EQ(1, Reduce(one.begin(), one.end(), 0, Summator<int>()));

    std::vector<int> two{1, 2};
    ASSERT_EQ(3, Reduce(two.begin(), two.end(), 0, Summator<int>()));
}

TEST(Correctness, SimpleTest) {
    std::vector<uint32_t> lst(GenTest<uint32_t>(1000));

    auto func = [](uint32_t cur, uint32_t next) { return cur + next; };
    ASSERT_EQ(std::accumulate(lst.begin(), lst.end(), 0, func),
              Reduce(lst.begin(), lst.end(), 0, func));
}

TEST(VectorBool, Tricky) {
    // See https://stackoverflow.com/questions/33617421/write-concurrently-vectorbool
    std::vector<bool> go(GenTest<bool>(1000));
    auto func = [](bool cur, bool next) { return cur && next; };
    ASSERT_EQ(std::accumulate(go.begin(), go.end(), true, func),
              Reduce(go.begin(), go.end(), true, func));
}

template <class RandomAccessIterator, class T, class Func>
__attribute__((noinline)) T CanonicalReduce(RandomAccessIterator first, RandomAccessIterator last,
                                            const T& initial_value, Func func) {
    auto cur_value(initial_value);
    while (first != last) {
        cur_value = func(cur_value, *first++);
    }
    return cur_value;
}

uint32_t Gcd(uint32_t a, uint32_t b) {
    return !b ? a : Gcd(b, a % b);
}

// That's a bad test, don't write this in production. Also if it does not work,
// it may work on a server.
#ifdef NDEBUG
TEST(Perf, BetterReduce) {
    using namespace std::literals;
    std::vector<uint32_t> lst(GenTest<uint32_t>(1000 * 1000 * 100));

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto left = Reduce(lst.begin(), lst.end(), 0, Gcd);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto time_optimized = (end - begin) / 1ms;

    begin = std::chrono::steady_clock::now();
    auto right = CanonicalReduce(lst.begin(), lst.end(), 0, Gcd);
    end = std::chrono::steady_clock::now();
    auto time_simple = (end - begin) / 1ms;
    ASSERT_EQ(left, right);
    // At least 50% faster.
    ASSERT_GE(time_simple, 2 * time_optimized);
}
#endif
