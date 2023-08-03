#include <catch.hpp>
#include <util.h>
#include <fold.h>

#include <algorithm>
#include <list>

TEST_CASE("JustSum") {
    std::list<int> a{1, 2, 3, 6};
    REQUIRE(12 == Fold(a.cbegin(), a.cend(), 0, Sum()));
}

TEST_CASE("JustProd") {
    std::list<int> a{-1, 1, 2, 3};
    REQUIRE(-6 == Fold(a.cbegin(), a.cend(), 1, Prod()));
}

TEST_CASE("Vectors") {
    std::vector<std::vector<int>> v{{1, 2}, {3}, {4, 5, 6}, {}};
    std::vector<int> expected{1, 2, 3, 4, 5, 6};
    REQUIRE(expected == Fold(v.begin(), v.end(), std::vector<int>(), Concat()));
}

TEST_CASE("SequenceLength") {
    {
        std::list<int> a{1, 3, -5, 4};
        int cnt = 0;
        Length f(&cnt);
        Fold(a.begin(), a.end(), 0, f);
        REQUIRE(4 == cnt);
    }
    {
        std::list<std::string> a{"aba", "caba"};
        int cnt = 0;
        Length f(&cnt);
        Fold(a.begin(), a.end(), std::string(), f);
        REQUIRE(2 == cnt);
    }
}
