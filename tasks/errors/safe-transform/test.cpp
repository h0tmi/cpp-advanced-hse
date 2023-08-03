#include <vector>
#include <string>
#include <cctype>
#include <forward_list>
#include <stdexcept>
#include <deque>
#include <iostream>

#include <catch.hpp>
#include <transform.h>

TEST_CASE("Transform works 1") {
    std::vector<int> data{1, 2, 3, 4};
    TransformIf(
        data.begin(), data.end(), [](int x) { return x % 2 == 0; }, [](int& x) { x *= x; });
    std::vector<int> expected{1, 4, 3, 16};
    REQUIRE(expected == data);
}

TEST_CASE("Transform works 2") {
    std::string s("abacaba");
    TransformIf(
        s.begin(), s.end(), [](char) { return true; }, [](char& c) { c = std::toupper(c); });
    REQUIRE(s == "ABACABA");
}

TEST_CASE("Transform works 3") {
    std::vector<int> data{1, 2, 0, 5, 42};
    TransformIf(
        data.begin(), data.end(), [](int x) { return x != 0; }, [](int& x) { x = 42 / x; });
    std::vector<int> expected{42, 21, 0, 8, 1};
    REQUIRE(data == expected);
}

TEST_CASE("Predicate throws") {
    {
        std::vector<std::string> data{"aba", "caba"};
        std::vector<std::string> expected{"aba", "caba"};
        REQUIRE_THROWS_AS(TransformIf(
                              data.begin(), data.end(),
                              [](const std::string& cur) {
                                  if (cur.size() == 4) {
                                      throw std::string("don't do this");
                                  }
                                  return true;
                              },
                              [](std::string& cur) { cur.erase(0, 1); }),
                          std::string);
        REQUIRE(data == expected);
    }
    {
        std::forward_list<double> vals{-5.0, -3.0, 1.0, -4.0};
        auto expected = vals;
        REQUIRE_THROWS_AS(TransformIf(
                              vals.begin(), vals.end(),
                              [](double x) {
                                  if (x > 0) {
                                      throw std::runtime_error("Positive value");
                                  }
                                  return true;
                              },
                              [](double& x) { x = -x; }),
                          std::runtime_error);
        REQUIRE(expected == vals);
    }
}

TEST_CASE("Functor throws") {
    std::vector<std::string> data{"a", "b", "caba", "hello", "TEST"};
    auto expected = data;
    REQUIRE_THROWS_AS(
        TransformIf(
            data.begin(), data.end(), [](const std::string& s) { return s.size() > 2; },
            [](std::string& s) {
                s[2] = std::toupper(s[2]);
                if (s.size() == 5) {
                    throw 42;
                }
            }),
        int);
    REQUIRE(expected == data);
}

struct Int {
    int x;
    Int() = default;
    Int(int x) : x(x) {
    }
    Int(const Int& rhs) {
        UpdateCounter();
        x = rhs.x;
    }
    Int& operator=(const Int& rhs) {
        UpdateCounter();
        x = rhs.x;
        return *this;
    }

    Int(Int&&) = delete;
    Int& operator=(Int&&) = delete;

    static int counter;
    static int max_value;

    static void Reset(int max_val) {
        counter = 0;
        max_value = max_val;
    }

    static void UpdateCounter() {
        ++counter;
        if (counter == max_value) {
            throw std::logic_error("Fail");
        }
    }

    bool operator==(const Int& rhs) const {
        return x == rhs.x;
    }
};
int Int::counter = 0;
int Int::max_value = 0;

std::ostream& operator<<(std::ostream& os, const Int& i) {
    return os << i.x;
}

TEST_CASE("Copy fails") {
    Int::Reset(100);
    std::vector<Int> data{Int(1), Int(2), Int(3), Int(4), Int(5)};
    Int::Reset(3);
    TransformIf(
        data.begin(), data.end(), [](const Int&) { return true; },
        [](Int& val) { val.x *= val.x; });
    for (size_t i = 0; i < data.size(); ++i) {
        REQUIRE(static_cast<size_t>(data[i].x) == (i + 1) * (i + 1));
    }
}

TEST_CASE("All fails") {
    Int::Reset(100);
    std::vector<Int> data{Int(3), Int(5), Int(2)};
    Int::Reset(2);
    REQUIRE_THROWS_AS(TransformIf(
                          data.begin(), data.end(), [](const Int&) { return true; },
                          [](Int& val) {
                              val.x *= 2;
                              if (val.x == 10) {
                                  throw std::range_error("too big");
                              }
                          }),
                      std::exception);
}

TEST_CASE("Copy predicate only") {
    Int::Reset(100);
    std::vector<Int> data{Int(1), Int(2), Int(3)};
    auto expected = data;
    Int::Reset(3);
    REQUIRE_THROWS_AS(TransformIf(
                          data.begin(), data.end(),
                          [](const Int& val) {
                              if (val.x == 3) {
                                  throw 42;
                              }
                              return val.x == 2;
                          },
                          [](Int& val) { val.x += 2; }),
                      int);
    REQUIRE(expected == data);
}

TEST_CASE("Stress") {
    std::deque<Int> data;
    const int len = 1e6;
    const int iterations = 1e4;
    for (int i = 0; i < len; ++i) {
        data.emplace_back(i);
    }

    const int max_count = 100;

    for (int i = 0; i < iterations; ++i) {
        Int::Reset(100500);
        auto predicate = [](const Int& val) { return val.x % 2 == 0; };

        REQUIRE_THROWS_AS(TransformIf(data.begin(), data.end(), predicate,
                                      [](Int& val) {
                                          if (val.x++ == max_count) {
                                              throw std::runtime_error("Stop");
                                          }
                                      }),
                          std::runtime_error);
        for (int j = 0; j <= max_count; ++j) {
            REQUIRE(data[j].x == j);
        }
    }
}
