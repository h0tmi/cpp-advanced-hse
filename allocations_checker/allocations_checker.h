#pragma once

#include <cstddef>

namespace alloc_checker {

std::size_t AllocCount();

std::size_t DeallocCount();

void ResetCounters();

}  // namespace alloc_checker

#define EXPECT_ZERO_ALLOCATIONS(X)                     \
    do {                                               \
        auto __xxx = alloc_checker::AllocCount();      \
        X;                                             \
        REQUIRE(alloc_checker::AllocCount() == __xxx); \
    } while (0)

#define EXPECT_ONE_ALLOCATION(X)                           \
    do {                                                   \
        auto __xxx = alloc_checker::AllocCount();          \
        X;                                                 \
        REQUIRE(alloc_checker::AllocCount() == __xxx + 1); \
    } while (0)

#define EXPECT_NO_MORE_THAN_ONE_ALLOCATION(X)              \
    do {                                                   \
        auto __xxx = alloc_checker::AllocCount();          \
        X;                                                 \
        REQUIRE(alloc_checker::AllocCount() <= __xxx + 1); \
    } while (0)
