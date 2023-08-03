#include <gtest/gtest.h>
#include <iostream>
#include "is_prime.h"

TEST(basic_tests, just_test) {
    ASSERT_TRUE(IsPrime(2));
    ASSERT_FALSE(IsPrime(1));
    ASSERT_FALSE(IsPrime(0));
    ASSERT_TRUE(IsPrime(17239));
    ASSERT_FALSE(IsPrime(9));

    int mul = 100 * 100 * 100 + 3;
    ASSERT_TRUE(IsPrime(mul));
    ASSERT_FALSE(IsPrime(static_cast<uint64_t>(mul) * mul));
}
