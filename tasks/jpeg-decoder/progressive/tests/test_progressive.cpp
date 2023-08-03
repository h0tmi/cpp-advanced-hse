#include <test_commons.hpp>

#include <catch.hpp>

TEST_CASE("Progressive jfif (grayscale)", "[jpg]") {
    CheckImage("progressive.jpg");
}

TEST_CASE("Progressive jfif (4:4:4)", "[jpg]") {
    CheckImage("progressive_small.jpg");
}

TEST_CASE("Progressive jfif/exif (4:2:2)", "[jpg]") {
    CheckImage("progressive-2.jpg", "such decoder");
}
