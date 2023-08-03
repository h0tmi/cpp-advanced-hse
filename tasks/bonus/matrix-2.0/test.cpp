#include <catch.hpp>

#include <matrix.h>

#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <random>
#include <deque>
#include <type_traits>
#include <string>

struct Int {
    int64_t x;
    Int() {
        x = 0;
    }

    Int(int64_t x) : x(x) {
    }

    static int mul_counter;
    static void Reset() {
        mul_counter = 0;
    }

    Int& operator+=(Int rhs) {
        x += rhs.x;
        return *this;
    }

    Int& operator-=(Int rhs) {
        x -= rhs.x;
        return *this;
    }

    Int& operator*=(Int rhs) {
        x *= rhs.x;
        ++mul_counter;
        return *this;
    }
};
int Int::mul_counter = 0;

Int operator*(Int a, Int b) {
    ++Int::mul_counter;
    return Int{a.x * b.x};
}

Int operator+(Int a, Int b) {
    return Int{a.x + b.x};
}

Int operator-(Int a, Int b) {
    return Int{a.x - b.x};
}

Int operator-(Int a) {
    return Int{-a.x};
}

TEST_CASE("Basic ops") {
    Matrix<int> left(2, 3);
    Matrix<int> right(2, 3);
    Matrix<int> sub(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            left(i, j) = i;
            right(i, j) = j;
            sub(i, j) = i == j;
        }
    }
    Matrix test = left + right - sub;
    REQUIRE(test.Rows() == 2);
    REQUIRE(test.Columns() == 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            REQUIRE(test(i, j) == i + j - (i == j));
        }
    }
}

TEST_CASE("Multipling") {
    Matrix<int> left{{1, 2, 3}, {4, 5, 6}};
    Matrix<int> right{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    Matrix<int> test = left * right;
    std::vector<std::vector<int>> expected{{38, 44, 50, 56}, {83, 98, 113, 128}};

    REQUIRE(test.Rows() == 2);
    REQUIRE(test.Columns() == 4);
    for (size_t i = 0; i < test.Rows(); ++i) {
        for (size_t j = 0; j < test.Columns(); ++j) {
            REQUIRE(test(i, j) == expected[i][j]);
        }
    }
}

TEST_CASE("Mixed ops") {
    Matrix<uint64_t> left{{1, 1}, {1, 1}};
    const auto inf = std::numeric_limits<uint64_t>::max();
    Matrix<uint64_t> right{{inf, inf}, {inf, inf}};
    Matrix<uint64_t> mul{{1, 2}, {3, 4}};

    Matrix<uint64_t> result = mul * (left + right);
    REQUIRE(result.Rows() == 2);
    REQUIRE(result.Columns() == 2);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            REQUIRE(result(i, j) == 0);
        }
    }
}

TEST_CASE("Simple order") {
    Int::Reset();
    Matrix<Int> first(50, 5);
    Matrix<Int> second(5, 100);
    Matrix<Int> third(100, 10);
    Matrix<Int> result = first * second * third;
    REQUIRE(Int::mul_counter == 7500);
}

TEST_CASE("Exceptions") {
    Matrix<int> first(2, 3);
    Matrix<int> second(3, 4);
    Matrix<int> third(4, 6);
    Matrix<int> fourth(5, 7);
    REQUIRE_THROWS_AS(Matrix<int>(first * second * third * fourth), std::runtime_error);
}

TEST_CASE("Chain") {
    Int::Reset();
    Matrix<Int> a(40, 20);
    Matrix<Int> b(20, 30);
    Matrix<Int> c(30, 10);
    Matrix<Int> d(10, 30);
    Matrix<Int> e(40, 20);
    Matrix<Int> f(20, 30);
    Matrix<Int> result = a * b * c * d + e * f;
    REQUIRE(Int::mul_counter == 26000 + 24000);
}

Matrix<Int> TestMul(const Matrix<Int>& lhs, const Matrix<Int>& rhs) {
    Matrix<Int> result(lhs.Rows(), rhs.Columns());
    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < rhs.Columns(); ++j) {
            Int sum = 0;
            for (size_t k = 0; k < lhs.Columns(); ++k) {
                sum += lhs(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

TEST_CASE("Stress") {
    std::mt19937 gen(7835675);
    std::uniform_int_distribution<size_t> dist_sizes(5, 100);
    std::uniform_int_distribution<int> dist_elems(-5, 5);
    const int matrix_count = 10;
    std::vector<int64_t> ok_counts{528658, 146820, 129235, 148710, 179753,
                                   233154, 312672, 130800, 546624, 165880};
    for (int test_num = 0; test_num < 10; ++test_num) {
        std::vector<size_t> sizes(matrix_count + 1);
        for (auto& cur : sizes) {
            cur = dist_sizes(gen);
        }

        std::deque<Matrix<Int>> m;
        for (int q = 0; q < matrix_count; ++q) {
            Matrix<Int> cur(sizes[q], sizes[q + 1]);
            for (size_t i = 0; i < cur.Rows(); ++i) {
                for (size_t j = 0; j < cur.Columns(); ++j) {
                    cur(i, j).x = dist_elems(gen);
                }
            }
            m.push_back(std::move(cur));
        }

        Int::Reset();
        Matrix<Int> ok(m[0]);
        for (int i = 1; i < matrix_count; ++i) {
            ok = TestMul(ok, m[i]);
        }

        Int::Reset();
        Matrix<Int> test = m[0] * m[1] * m[2] * m[3] * m[4] * m[5] * m[6] * m[7] * m[8] * m[9];
        auto ok_count = Int::mul_counter;
        REQUIRE(ok_count == ok_counts[test_num]);
        REQUIRE(ok.Rows() == test.Rows());
        REQUIRE(ok.Columns() == test.Columns());

        for (size_t i = 0; i < ok.Rows(); ++i) {
            for (size_t j = 0; j < ok.Columns(); ++j) {
                REQUIRE(ok(i, j).x == test(i, j).x);
            }
        }
    }
}

template <class T, class = decltype(std::declval<T>() * std::declval<T>())>
std::false_type CheckGlue(const T&);
std::true_type CheckGlue(...);

TEST_CASE("Glue doesn't break *") {
    std::string dummy;
    static_assert(decltype(CheckGlue(dummy))::value, "Your * is broken");
}
