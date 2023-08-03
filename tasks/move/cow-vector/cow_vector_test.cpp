#include <catch.hpp>

#include <cow_vector.h>

TEST_CASE("Simple vector operations") {
    COWVector v;

    REQUIRE(v.Size() == 0);

    v.Resize(3);
    REQUIRE(v.Get(0).empty());

    v.Set(2, "foo");
    v.Set(1, "bar");

    REQUIRE(v.Get(2) == "foo");
    REQUIRE(v.Get(1) == "bar");

    REQUIRE(v.Back() == "foo");

    v.PushBack("zog");
    REQUIRE(v.Size() == 4);
    REQUIRE(v.Back() == "zog");
}

TEST_CASE("COW semantics") {
    COWVector v1;
    v1.PushBack("foo");

    auto p1 = &v1.Get(0);

    SECTION("PushBack unshares state") {
        COWVector v2{v1};
        REQUIRE(&v2.Get(0) == p1);

        v2.PushBack("bar");
        REQUIRE(&v2.Get(0) != p1);
        REQUIRE(v2.Get(0) == "foo");
    }

    SECTION("Set unshares state") {
        COWVector v3{v1};
        v3.Set(0, "bar");
        auto p2 = &v3.Get(0);
        REQUIRE(p2 != p1);

        v3.Set(0, "zog");
        REQUIRE(p2 == &v3.Get(0));
    }

    SECTION("Copy assignment frees old state") {
        COWVector v4;
        v4.PushBack("bar");
        v4.PushBack("zog");
        COWVector v5(v4);

        v4 = v1;
        REQUIRE(v4.Get(0) == "foo");
        REQUIRE(&v4.Get(0) == p1);
        REQUIRE(v5.Get(0) == "bar");
        REQUIRE(v5.Get(1) == "zog");
    }

    SECTION("Read operation do not copy state") {
        COWVector v5{v1};

        REQUIRE(&v5.Get(0) == p1);

        REQUIRE(v5.Size() == 1);
        REQUIRE(&v5.Get(0) == p1);
    }
}

TEST_CASE("Correct reallocation") {
    COWVector v1;
    v1.PushBack("foo");
    v1.PushBack("foo");
    v1.PushBack("foo");
}
