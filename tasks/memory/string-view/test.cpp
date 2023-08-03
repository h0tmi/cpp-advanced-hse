#include <catch.hpp>
#include <util.h>
#include <string_view.h>

#include <string>

TEST_CASE("Constructors") {
    {
        StringView s("abacaba");
        REQUIRE('c' == s[3]);
        REQUIRE(7u == s.Size());
    }
    {
        StringView s("caba", 3);
        REQUIRE('c' == s[0]);
        REQUIRE(3u == s.Size());
    }
    {
        std::string a("abacaba");
        StringView s(a);
        REQUIRE('c' == s[3]);
        REQUIRE(7u == s.Size());
    }
    {
        std::string a("abacaba");
        StringView s(a, 3);
        REQUIRE('c' == s[0]);
        REQUIRE(4u == s.Size());
    }
    {
        std::string a("abacaba");
        StringView s(a, 3, 3);
        REQUIRE('a' == s[1]);
        REQUIRE(3u == s.Size());
    }
}

TEST_CASE("Constness") {
    std::string a("abacaba");
    const StringView s(a, 3, 3);
    REQUIRE('b' == s[2]);
    REQUIRE(3u == s.Size());
}

TEST_CASE("Big") {
    RandomGenerator rnd(38545678);
    const int count = 1e5;
    auto a = rnd.GenString(count);
    StringView s(a, 1);

    for (int i = 0; i < count; ++i) {
        int ind = rnd.GenInt(1, count - 1);
        a[ind] = rnd.GenInt<int>('a', 'z');
        REQUIRE(a[ind] == s[ind - 1]);
    }

    REQUIRE(count - 1 == static_cast<int>(s.Size()));
}
