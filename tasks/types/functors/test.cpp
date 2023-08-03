#include <catch.hpp>
#include <util.h>
#include <functors.h>

#include <algorithm>
#include <vector>

TEST_CASE("Binary") {
    std::vector<int> a{1, 3, 4, 0, -1, 6};
    auto less = [](int x, int y) { return x < y; };
    std::sort(a.begin(), a.end(), MakeReverseBinaryFunctor(less));

    std::vector<int> expected{6, 4, 3, 1, 0, -1};
    REQUIRE(expected == a);
}

TEST_CASE("String") {
    std::vector<std::string> a{"aba", "caba"};
    auto greater = [](const std::string& a, const std::string& b) { return a > b; };
    std::sort(a.begin(), a.end(), MakeReverseBinaryFunctor(greater));

    std::vector<std::string> expected{"aba", "caba"};
    REQUIRE(expected == a);
}

static bool IsEven(int x) {
    return x % 2 == 0;
}

TEST_CASE("Unary") {
    std::vector<int> a{1, 2, 0, -1, 3, 4};
    auto it = std::remove_if(a.begin(), a.end(), MakeReverseUnaryFunctor(IsEven));
    a.resize(std::distance(a.begin(), it));

    std::vector<int> expected{2, 0, 4};
    REQUIRE(expected == a);
}
