#include <catch.hpp>

#include <ugly.h>

struct TestCase {
    std::vector<double> x, y;
    std::vector<std::pair<double, double>> checks;
};

/* clang-format off */

TEST_CASE("Spline interpolation works") {
    std::vector<TestCase> test_cases = {
        {
            {0.0, 1.0, 2.0},
            {0.0, 0.0, 0.0},
            {
                {0.0, 0.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {1.5, 0.0},
                {2.0, 0.0}
            }
        },
        {
            {0.0, 1.0, 2.0},
            {0.0, 1.0, 2.0},
            {
                {0.0, 0.0},
                {0.5, 0.5},
                {1.0, 1.0},
                {1.5, 1.5},
                {2.0, 2.0}
            }
        },
        {
            {0.0, 1.0, 2.0, 3.0},
            {0.0, 2.71, 3.14, 1.1},
            {
                {0.0, 0.0},
                {0.5, 1.52125},
                {1.0, 2.71},
                {1.5, 3.28125},
                {2.0, 3.14},
                {2.5, 2.31},
                {3.0, 1.1}
            }
        }
    };

    for (const auto& test : test_cases) {
        Spline spline(test.x, test.y, 0.0, 0.0);

        for (auto check : test.checks) {
            REQUIRE(check.second == spline.Interpolate(check.first));
        }
    }
}
