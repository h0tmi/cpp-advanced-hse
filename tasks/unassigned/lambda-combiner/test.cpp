#include "lamda_combiner.h"

#include <catch.hpp>
#include <vector>
#include <algorithm>

TEST_CASE("Sum doubles vs sum ints") {
    std::vector<double> doubles = {0.2, 0.35, -8.34, 1.78};
    std::vector<int> ints = {-9, -4, -456, 23, 598, -117, 43};

    double sum_d = 0;
    auto l1 = [&sum_d](double &num) { sum_d += num; };

    double sum_i = 0;
    auto l2 = [&sum_i](int &num) { sum_i += num; };

    auto combined = LambdaCombiner(l1, l2);

    std::for_each(doubles.begin(), doubles.end(), combined);
    std::for_each(ints.begin(), ints.end(), combined);

    REQUIRE(sum_d == -6.01);
    REQUIRE(sum_i == 78);
}

TEST_CASE("Change ints vs count strings") {
    int maxx = 3;
    std::vector<int> ints = {45, 2, 3, -1, 5, 3, 0, 11};
    std::vector<std::string> strings = {"hi", "!", "i", "really", "like", "cpp", "<3"};

    auto l1 = [maxx](int &elem) { elem = std::min(elem, maxx); };

    auto l2 = [&ints](std::string &elem) { return elem.size() > ints[elem.size() % ints.size()]; };

    auto combined = LambdaCombiner(l1, l2);

    std::for_each(ints.begin(), ints.end(), combined);
    REQUIRE(ints[0] == 3);
    REQUIRE(ints[1] == 2);
    REQUIRE(ints[2] == 3);
    REQUIRE(ints[3] == -1);
    REQUIRE(ints[4] == 3);
    REQUIRE(ints[5] == 3);
    REQUIRE(ints[6] == 0);
    REQUIRE(ints[7] == 3);

    REQUIRE(std::count_if(strings.begin(), strings.end(), combined) == 2);
}
