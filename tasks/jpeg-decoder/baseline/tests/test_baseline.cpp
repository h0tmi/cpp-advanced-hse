#include <test_commons.hpp>

#include <catch.hpp>

#include <string>

TEST_CASE("small jfif (4:2:0)", "[jpg]") {
    CheckImage("small.jpg", ":)");
}

TEST_CASE("jfif (4:4:4)", "[jpg]") {
    CheckImage("lenna.jpg");
}

TEST_CASE("jfif (4:2:2)", "[jpg]") {
    CheckImage("bad_quality.jpg", "so quality");
}

TEST_CASE("tiny jfif (4:4:4)", "[jpg]") {
    CheckImage("tiny.jpg");
}

TEST_CASE("exif (4:2:2)", "[jpg]") {
    CheckImage("chroma_halfed.jpg");
}

TEST_CASE("exif (grayscale)", "[jpg]") {
    CheckImage("grayscale.jpg");
}

TEST_CASE("jfif/exif (4:2:0)", "[jpg]") {
    CheckImage("test.jpg");
}

TEST_CASE("exif (4:4:4)", "[jpg]") {
    CheckImage("colors.jpg");
}

TEST_CASE("photoshop (4:4:4)", "[jpg]") {
    CheckImage("save_for_web.jpg");
}

TEST_CASE("prostitute", "[jpg]") {
    CheckImage("prostitute.jpg");
}

TEST_CASE("architecture", "[jpg]") {
    CheckImage("architecture.jpg");
}

TEST_CASE("witch", "[jpg]") {
    CheckImage("witch.jpg");
}

TEST_CASE("Error handling", "[jpg]") {
    const size_t tests_count = 24;
    for (size_t i = 1; i <= tests_count; ++i) {
        ExpectFail("bad" + std::to_string(i) + ".jpg");
    }
}
