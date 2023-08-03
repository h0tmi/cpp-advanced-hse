#include <catch.hpp>
#include <multiplication.h>

TEST_CASE("Simple") {
    REQUIRE(6 == Multiply(2, 3));
}

TEST_CASE("Advanced") {
    REQUIRE(-100000018299999867LL == Multiply(999999993, -100000019));
}
