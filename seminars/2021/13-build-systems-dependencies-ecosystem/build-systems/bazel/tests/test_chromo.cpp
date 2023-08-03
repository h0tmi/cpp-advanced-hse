#include "chromo/time.hpp"

#include <gtest/gtest.h>

TEST(ChromoTests, Now) {
    EXPECT_EQ(chromo::Now().since_unix_epoch, 42);
}
