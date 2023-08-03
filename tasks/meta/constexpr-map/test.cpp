#include <constexpr_map.h>
#include <constexpr_string.h>
#include <sort.h>

#include <array>

#include <catch.hpp>

constexpr auto MapInsert() {
    ConstexprMap<ConstexprString, int> a;
    a["check"] = 2;
    a["x"] = 3;
    a["4"] = 1;
    return a;
}

constexpr void CheckInsert() {
    constexpr auto test = MapInsert();
    static_assert(test.Find("check"));
    static_assert(test.Find("x"));
    static_assert(test.Find("4"));
    static_assert(!test.Find("aba"));
    static_assert(test.Size() == 3u);
}

constexpr auto MapErase() {
    ConstexprMap<int, ConstexprString, 2> a;
    a[17239] = "5";
    a[-3] = "7";
    a.Erase(17239);
    a[8] = "end";
    return a;
}

constexpr void CheckErase() {
    constexpr auto test = MapErase();
    static_assert(test.Find(-3));
    static_assert(test[8] == "end");
    static_assert(!test.Find(17239));
    static_assert(test.Size() == 2u);
}

constexpr void CheckOrder() {
    {
        constexpr auto test = MapInsert();
        static_assert(test.GetByIndex(0) == std::make_pair(ConstexprString("check"), 2));
        static_assert(test.GetByIndex(1) == std::make_pair(ConstexprString("x"), 3));
        static_assert(test.GetByIndex(2) == std::make_pair(ConstexprString("4"), 1));
    }
    {
        constexpr auto test = MapErase();
        static_assert(test.GetByIndex(0) == std::make_pair(-3, ConstexprString("7")));
        static_assert(test.GetByIndex(1) == std::make_pair(8, ConstexprString("end")));
    }
}

constexpr void CheckSort() {
    {
        constexpr auto a = MapInsert();
        constexpr auto test = Sort(a);
        static_assert(test.GetByIndex(0) == std::make_pair(ConstexprString("4"), 1));
        static_assert(test.GetByIndex(1) == std::make_pair(ConstexprString("check"), 2));
        static_assert(test.GetByIndex(2) == std::make_pair(ConstexprString("x"), 3));
    }
    {
        constexpr auto a = MapErase();
        constexpr auto test = Sort(a);
        static_assert(test.GetByIndex(0) == std::make_pair(8, ConstexprString("end")));
        static_assert(test.GetByIndex(1) == std::make_pair(-3, ConstexprString("7")));
    }
}

TEST_CASE("Compile") {
    CheckInsert();
    CheckErase();
    CheckOrder();
    CheckSort();
}
