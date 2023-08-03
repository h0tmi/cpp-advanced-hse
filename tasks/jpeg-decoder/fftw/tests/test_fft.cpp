#include <fft.h>

#include <catch.hpp>

TEST_CASE("Dimensions check") {
    std::vector<double> input;
    std::vector<double> output;
    REQUIRE_THROWS_AS(DctCalculator(4, &input, &output), std::invalid_argument);
    input.resize(16);
    REQUIRE_THROWS_AS(DctCalculator(4, &input, &output), std::invalid_argument);
    output.resize(16);
    input.clear();
    REQUIRE_THROWS_AS(DctCalculator(4, &input, &output), std::invalid_argument);
}

/*
TEST_CASE("IDCT Sanity check") {
    std::vector<double> input;
    input.resize(4);
    std::vector<double> output;
    output.resize(4);
    DctCalculator dst(2, &input, &output);
    input = {
        6,
        -1,
        5,
        4,
    };
    const std::vector<double> canon = {1.75, 1, -0.5, 0.75};
    dst.Inverse();
    for (int i = 0; i < 4; ++i) {
        REQUIRE(output[i] == Approx(canon[i]));
    }
}
*/

TEST_CASE("IDCT Check") {
    std::vector<double> input;
    input.resize(64);
    std::vector<double> output;
    output.resize(64);
    DctCalculator dst(8, &input, &output);
    input = {298, 2,  2,  1, 2, 1, -1, 4,  0, -1, 2, -1, -4, -2, 1,  -3, 2, 0,  0,  0,  -1, 3,
             -1,  -3, -2, 0, 0, 0, 1,  2,  0, 0,  0, 1,  0,  1,  -1, 0,  1, -1, -2, -1, -3, -2,
             -2,  2,  1,  1, 0, 3, -1, -2, 0, 0,  1, 0,  -1, 0,  -1, -1, 0, 1,  1,  -2};
    const std::vector<double> canon = {
        37.186,  37.4825, 38.1485, 37.9803, 33.8103, 37.851,  37.8502, 36.1053, 39.2241, 39.4683,
        35.099,  35.3723, 37.6809, 37.6188, 39.2842, 39.4463, 36.9093, 38.2059, 39.2389, 36.6495,
        37.0889, 36.283,  36.0244, 37.5641, 37.1725, 36.8161, 38.3208, 34.0217, 39.0697, 36.3035,
        37.5236, 36.7656, 39.4716, 36.3931, 35.8898, 35.6998, 38.1884, 35.8384, 35.995,  37.3041,
        39.6835, 35.8234, 37.7383, 36.6318, 39.4474, 33.141,  36.9215, 36.4844, 37.2246, 33.7804,
        37.0466, 38.3393, 38.2591, 35.1389, 39.1874, 35.9945, 41.0395, 37.9654, 37.293,  38.6272,
        38.9895, 36.2726, 38.0565, 36.5684};
    dst.Inverse();
    for (int i = 0; i < 64; ++i) {
        REQUIRE(output[i] == Approx(canon[i]));
    }
}
