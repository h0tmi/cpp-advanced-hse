#include <dedup.h>

#include <catch.hpp>

#include <string>
#include <memory>

using std::string;

TEST_CASE("Duplicating strings") {
    auto i0 = std::make_shared<string>("a");
    auto i1 = std::make_shared<string>("b");
    auto i2 = std::make_shared<string>("c");

    std::vector<std::shared_ptr<string>> shared = {i0, i1, i2, i2, i1, i0};

    auto unshared = Duplicate(shared);

    REQUIRE(*(unshared[0]) == "a");
    REQUIRE(*(unshared[1]) == "b");
    REQUIRE(*(unshared[2]) == "c");
    REQUIRE(*(unshared[3]) == "c");
    REQUIRE(*(unshared[4]) == "b");
    REQUIRE(*(unshared[5]) == "a");

    REQUIRE(unshared[0] != unshared[5]);
    REQUIRE(unshared[1] != unshared[4]);
    REQUIRE(unshared[2] != unshared[3]);

    REQUIRE(unshared[0].get() != i0.get());
    REQUIRE(unshared[1].get() != i1.get());
    REQUIRE(unshared[2].get() != i2.get());
}

TEST_CASE("Deduping strings") {
    std::vector<std::unique_ptr<string>> unshared;
    unshared.emplace_back(std::make_unique<string>("a"));
    unshared.emplace_back(std::make_unique<string>("b"));
    unshared.emplace_back(std::make_unique<string>("c"));
    unshared.emplace_back(std::make_unique<string>("c"));
    unshared.emplace_back(std::make_unique<string>("b"));
    unshared.emplace_back(std::make_unique<string>("a"));

    auto shared = DeDuplicate(unshared);
    REQUIRE(*(shared[0]) == "a");
    REQUIRE(*(shared[1]) == "b");
    REQUIRE(*(shared[2]) == "c");
    REQUIRE(*(shared[3]) == "c");
    REQUIRE(*(shared[4]) == "b");
    REQUIRE(*(shared[5]) == "a");

    REQUIRE(shared[0] == shared[5]);
    REQUIRE(shared[1] == shared[4]);
    REQUIRE(shared[2] == shared[3]);
}

TEST_CASE("More duplication") {
    auto item = std::make_shared<string>("abacaba");
    std::vector<std::shared_ptr<string>> shared = {item, item, item};

    auto unshared = Duplicate(shared);
    REQUIRE(unshared[0] != unshared[1]);
    REQUIRE(unshared[1] != unshared[2]);

    for (int i = 0; i < 3; ++i) {
        REQUIRE(*unshared[i] == "abacaba");
    }
}
