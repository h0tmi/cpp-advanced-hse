#include <gtest/gtest.h>
#include <util.h>

#include "find_subsets.h"
#include "commons.h"

#include <unordered_set>

void CheckSubsets(const std::vector<int64_t>& data, const std::vector<size_t>& first_indices,
                  const std::vector<size_t>& second_indices) {
    ASSERT_FALSE(first_indices.empty());
    ASSERT_FALSE(second_indices.empty());

    std::unordered_set<size_t> used_indices;
    int64_t first_sum = 0;
    for (auto cur : first_indices) {
        ASSERT_LT(cur, data.size());
        first_sum += data[cur];
        used_indices.insert(cur);
    }

    int64_t second_sum = 0;
    for (auto cur : second_indices) {
        ASSERT_LT(cur, data.size());
        second_sum += data[cur];
        ASSERT_FALSE(used_indices.count(cur));
    }

    ASSERT_EQ(first_sum, second_sum);
}

void CheckAnswer(const std::vector<int64_t>& data, const Subsets& result, bool answer_exists) {
    ASSERT_EQ(answer_exists, result.exists);
    if (answer_exists) {
        CheckSubsets(data, result.first_indices, result.second_indices);
    }
}

TEST(Correctness, Simple) {
    std::vector<int64_t> data{1, 2, 4, 8, 15};
    CheckAnswer(data, FindEqualSumSubsets(data), true);
}

TEST(Correctness, Empty) {
    std::vector<int64_t> data;
    CheckAnswer(data, FindEqualSumSubsets(data), false);
}

TEST(Correctness, EmptyPart) {
    std::vector<int64_t> data{5, -5};
    CheckAnswer(data, FindEqualSumSubsets(data), false);
}

TEST(Correctness, Single) {
    std::vector<int64_t> data{1};
    CheckAnswer(data, FindEqualSumSubsets(data), false);
}

TEST(Correctness, EqualElems) {
    std::vector<int64_t> data{2, 2, 3};
    CheckAnswer(data, FindEqualSumSubsets(data), true);
}

TEST(Correctness, Middle) {
    std::vector<int64_t> data{1, 3, 5, 12, 20, -1};
    CheckAnswer(data, FindEqualSumSubsets(data), true);
}

TEST(Correctness, Negative) {
    std::vector<int64_t> data{1, -1, 3, -3, 5, -5, 7, -7, 15, -15, 62};
    CheckAnswer(data, FindEqualSumSubsets(data), true);
}

TEST(Correctness, Random1) {
    RandomGenerator gen(274647556);
    for (int i = 0; i < 8; ++i) {
        auto data = FillData(gen, 15, i + 1);
        CheckAnswer(data, FindEqualSumSubsets(data), true);
    }
}

TEST(Correctness, Random2) {
    RandomGenerator gen(7453435);
    for (int i = 0; i < 20; ++i) {
        auto data = FillData(gen, 15, 5, 5);
        CheckAnswer(data, FindEqualSumSubsets(data), true);
    }
}
