#include "test_commons.hpp"

#include <catch.hpp>
#include <decoder.h>

#include "image.h"
#include "png_encoder.hpp"
#include "libjpg_reader.hpp"

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <optional>

int artifact_index = 0;
#ifdef HSE_ARTIFACTS_DIR
const std::string kArtifactsDir = HSE_ARTIFACTS_DIR;
#else
const std::string kArtifactsDir;
#endif

#ifndef HSE_TASK_DIR
#define HSE_TASK_DIR "."
#endif

std::string ConstructBasePath() {
    std::string result(HSE_TASK_DIR);
    return result;
}

const std::string kBasePath = ConstructBasePath();

int sqr(int x) {  // NOLINT
    return x * x;
}

double Distance(const RGB& lhs, const RGB& rhs) {
    return sqrt(sqr(lhs.r - rhs.r) + sqr(lhs.g - rhs.g) + sqr(lhs.b - rhs.b));
}

void Compare(const Image& actual, const Image& expected) {
    double max = 0;
    double mean = 0;
    REQUIRE(actual.Width() == expected.Width());
    REQUIRE(actual.Height() == expected.Height());
    for (size_t y = 0; y < actual.Height(); ++y) {
        for (size_t x = 0; x < actual.Width(); ++x) {
            auto actual_data = actual.GetPixel(y, x);
            auto expected_data = expected.GetPixel(y, x);
            auto diff = Distance(actual_data, expected_data);
            max = std::max(max, diff);
            mean += diff;
        }
    }

    mean /= actual.Width() * actual.Height();
    REQUIRE(mean <= 5);
}

void CheckImage(const std::string& filename, const std::string& expected_comment,
                std::optional<std::string> output_filename) {
    std::cerr << "Running " << filename << "\n";
    std::ifstream fin(kBasePath + "tests/" + filename);
    if (!fin.is_open()) {
        throw std::invalid_argument("Cannot open a file");
    }
    auto image = Decode(fin);
    fin.close();
    REQUIRE(image.GetComment() == expected_comment);
    if (output_filename.has_value() && kArtifactsDir.empty()) {
        WritePng(output_filename.value(), image);
    }
    if (!kArtifactsDir.empty()) {
        WritePng(kArtifactsDir + "/" + std::to_string(artifact_index++) + ".png", image);
    }
    auto ok_image = ReadJpg(kBasePath + "tests/" + filename);
    Compare(image, ok_image);
}

void ExpectFail(const std::string& filename) {
    std::cerr << "Running negative test " << filename << "\n";
    std::ifstream fin(kBasePath + "tests/bad/" + filename);
    if (!fin.is_open()) {
        throw std::invalid_argument("Cannot open a file");
    }
    CHECK_THROWS(Decode(fin));
}
