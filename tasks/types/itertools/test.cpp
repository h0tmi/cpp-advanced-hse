#include <catch.hpp>

#include <itertools.h>
#include <vector>
#include <list>
#include <iostream>
#include <deque>
#include <memory>

TEST_CASE("Range 1") {
    int i = 0;
    for (auto val : Range(5)) {
        REQUIRE(val == i++);
    }
    REQUIRE(i == 5);
}

TEST_CASE("Range 2") {
    int i = 2;
    for (auto val : Range(2, 7, 2)) {
        REQUIRE(val == i);
        i += 2;
    }
    REQUIRE(i == 8);
}

TEST_CASE("Range huge") {
    int64_t i = 1;
    for (auto val : Range(1ll, 1ll << 61)) {
        REQUIRE(i == val);
        ++i;
        if (i == 100) {
            break;
        }
    }
    REQUIRE(i == 100);
}

TEST_CASE("Zip equal") {
    std::vector<int> first{1, 3, 5};
    std::list<int> second{6, 4, 2};
    auto i1 = first.begin();
    auto i2 = second.begin();
    for (const auto& val : Zip(first, second)) {
        REQUIRE(val.first == *i1);
        REQUIRE(val.second == *i2);
        ++i1;
        ++i2;
    }
    REQUIRE(i1 == first.end());
    REQUIRE(i2 == second.end());
}

TEST_CASE("Zip short") {
    std::string s("abacaba");
    auto it = s.begin();
    int64_t i = 0;

    for (const auto& val : Zip(s, Range(1ll << 62))) {
        REQUIRE(val.first == *it);
        REQUIRE(val.second == i++);
        ++it;
    }
    REQUIRE(it == s.end());
    REQUIRE(i == s.size());
}

struct Int {
    int x;
    Int(int val) : x(val) {
    }

    bool operator==(const Int& rhs) const {
        return x == rhs.x;
    }
};

TEST_CASE("Zip some test") {

    std::deque<Int> first;
    first.emplace_back(5);
    first.emplace_back(4);
    first.emplace_back(3);

    int i = 0;
    auto it = first.begin();
    for (const auto& val : Zip(Range(3), first)) {
        REQUIRE(val.first == i++);
        REQUIRE(val.second == *it);
        ++it;
    }
    REQUIRE(i == 3);
    REQUIRE(it == first.end());
}

TEST_CASE("Group small") {
    std::vector<int> data{1, 1, 2, 2, 2, 3};
    std::vector<std::vector<int>> expected{{1, 1}, {2, 2, 2}, {3}};

    size_t i = 0;

    for (const auto& val : Group(data)) {
        auto it = expected[i].begin();
        for (const auto& elem : val) {
            REQUIRE(elem == *it);
            ++it;
        }
        REQUIRE(it == expected[i].end());
        ++i;
    }
    REQUIRE(i == expected.size());
}

TEST_CASE("Group empty") {
    std::vector<int> empty;
    int i = 0;
    for (const auto& val : Group(empty)) {
        std::ignore = val;
        ++i;
    }
    REQUIRE(i == 0);
}

TEST_CASE("Group some test") {
    std::list<Int> data;
    data.emplace_back(1);
    data.emplace_back(2);
    data.emplace_back(2);

    int total = 2;

    for (const auto& val : Group(data)) {
        --total;
        int key = (*val.begin()).x;
        int i = 0;
        for (const auto& elem : val) {
            REQUIRE(elem.x == key);
            ++i;
        }
        REQUIRE(i == key);
    }
    REQUIRE(total == 0);
}

TEST_CASE("Temporary iterator") {
    std::vector<int> vec = {0, 1, 2};
    auto range = Range(5);
    auto zip = Zip(vec, range);
    auto val = *zip.begin();
    REQUIRE(val.first == val.second);
}

TEST_CASE("No copying containers") {
    std::shared_ptr<int> common_item = std::make_shared<int>(42);
    std::vector<std::shared_ptr<int>> vec(100, common_item);
    std::list<std::shared_ptr<int>> list(100, common_item);

    for (const auto& val : Zip(vec, list)) {
        REQUIRE(val.first.get() == common_item.get());
        REQUIRE(val.second.get() == common_item.get());
        REQUIRE(common_item.use_count() < 220);
    }

    for (const auto& group : Group(vec)) {
        for (const auto& item : group) {
            REQUIRE(item == common_item);
            REQUIRE(common_item.use_count() < 220);
        }
    }
}
