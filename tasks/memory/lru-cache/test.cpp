#include <catch.hpp>
#include <util.h>
#include <lru_cache.h>

TEST_CASE("Set and get", "[LruCache]") {
    LruCache cache(10);

    std::string value;

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    REQUIRE(cache.Get("a", &value));
    REQUIRE("1" == value);
    REQUIRE(cache.Get("b", &value));
    REQUIRE("2" == value);
    REQUIRE(cache.Get("c", &value));
    REQUIRE("3" == value);

    REQUIRE(!cache.Get("d", &value));

    cache.Set("c", "4");
    REQUIRE(cache.Get("c", &value));
    REQUIRE("4" == value);
}

TEST_CASE("Eviction", "[LruCache]") {
    LruCache cache(2);
    std::string value;

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    REQUIRE(!cache.Get("a", &value));
    REQUIRE(cache.Get("b", &value));
    REQUIRE(cache.Get("c", &value));

    cache.Set("b", "4");
    cache.Set("c", "5");
    cache.Set("b", "6");

    cache.Set("e", "7");
    REQUIRE(!cache.Get("c", &value));
    REQUIRE(cache.Get("b", &value));
    REQUIRE(cache.Get("e", &value));

    cache.Get("b", &value);
    cache.Set("f", "8");
    REQUIRE(!cache.Get("e", &value));
    REQUIRE(cache.Get("b", &value));
    REQUIRE(cache.Get("f", &value));
}

TEST_CASE("Stress", "[LruCache]") {
    LruCache cache(100);
    std::string value;
    RandomGenerator random;

    for (size_t i = 0; i < 100000; ++i) {
        if (random.GenInt<uint32_t>() % 2 == 0) {
            cache.Set(std::to_string(random.GenInt<uint32_t>() % 500), "foo");
        } else {
            cache.Get(std::to_string(random.GenInt<uint32_t>() % 500), &value);
        }
    }
}
