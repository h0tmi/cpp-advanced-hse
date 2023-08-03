#include <test_commons.hpp>

#include <catch.hpp>

#include <chrono>
#include <iostream>

TEST_CASE("huge", "[jpg]") {
#ifdef NDEBUG
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    CheckImage("huge.jpg");
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    REQUIRE(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() <= 7);
#else
    std::cerr
        << "WARNING!: Build in release mode to test time, use -DCMAKE_BUILD_TYPE=RelWithDebInfo"
        << std::endl;
#endif
}
