#include <catch.hpp>

#include "functions.h"

#include <array>
#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <random>
#include <any>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

struct Empty {
    bool operator==(const Empty&) const { return true; }
};

struct SomeStruct {
    int a;
    float b;
    std::array<char, 16> c;
};

static size_t counter = 0;
struct StaticCounter {
    StaticCounter operator+(StaticCounter) const {
        ++counter;
        return StaticCounter{};
    }
    bool operator==(const StaticCounter&) const { return true; }

    static void Reset() {
        counter = 0;
    }
};

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

int Square(int x) { return x * x; }

Empty CreateEmpty(std::any) { return Empty{}; }

std::string MarkBonus(const std::string& bonus) {
    return (bonus == "functional-world") ? "fun-vee" : "humdrum-vee";
}

auto X10(char i) { return List(i, i, i, i, i, i, i, i, i, i); }
auto PseudoRandomModify(int i) { return List('0' + i, Empty{}, i * i * i); }
auto Destroy(int i) { return List(); }

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

#define STR(buffer) std::string(buffer)
#define CNT StaticCounter{}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

TEST_CASE("List") {
    auto empty = List();

    auto single_char = List('\t');
    auto ints = List(1, 2, 3);
    auto doubles = List(1.1, 2.2, 3.3, 4.4, 5.5);

    int i = 123456789;
    auto tuple_like = List("string", 'c');
    auto weirdo = List(SomeStruct{1, 2.0f, {"somestruct"}}, Empty{}, "weird...", std::vector<int>{5, 5, 5}, Empty{}, i);
};

TEST_CASE("Length") {
    REQUIRE(length(List()) == 0);
    REQUIRE(length(List('1', 10, "100")) == 3);
    REQUIRE(length(List(List(1), List('1', '2'), List("1", "2", "3"))) == 3);
    REQUIRE(length(List("I'm so huge... I guess")) == 1);
    
    auto list = List('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
    REQUIRE(length(list) == 8);
}

TEST_CASE("Accumulate") {
    auto list = List(1, 2, 3, 4);
    REQUIRE(accumulate(list, 0) == 10);
    REQUIRE(length(list) == 4); // list is unmodified

    REQUIRE(accumulate(List(1, 10, 100, 1000), 10000) == 11111);
    REQUIRE(accumulate(List(STR("or "), STR("Die")), STR("Solve ")) == "Solve or Die");

   accumulate(List(CNT, CNT, CNT, CNT, CNT, CNT, CNT, CNT, CNT, CNT), CNT);
   REQUIRE(counter == 10);
   StaticCounter::Reset();
}

/* Head & Tail have no need to compile for empty lists */
TEST_CASE("Head & Tail") {
    auto list = List(1, STR("2"), 'c', Empty{});
    REQUIRE(head(list) == 1);

    auto list1 = tail(list);
    REQUIRE(head(list1) == "2");
    REQUIRE(length(list1) == 3);

    auto list2 = tail(list1);
    REQUIRE(head(list2) == 'c');
    REQUIRE(length(list2) == 2);

    auto list3 = tail(list2);
    REQUIRE(head(list3) == Empty{});
    REQUIRE(length(list3) == 1);

    auto list4 = tail(list3);
    REQUIRE(length(list4) == 0);

    auto massive = List(List(List(List(List(List())))));
    REQUIRE(length(head(head(head(head(head(massive)))))) == 0);
}

TEST_CASE("For Each & Get") {
    auto silly = for_each(CreateEmpty, List("Enormous"));
    REQUIRE(head(silly) == Empty{});

    REQUIRE(length(for_each(Square, List())) == 0);
    
    auto squared = for_each(Square, List(10, 20, 30));
    REQUIRE(get<0>(squared) == Square(10));
    REQUIRE(get<1>(squared) == Square(20));
    REQUIRE(get<2>(squared) == Square(30));
    REQUIRE(get<0>(squared) == head(squared));
    REQUIRE(get<1>(squared) == head(tail(squared)));
    REQUIRE(get<2>(squared) == head(tail(tail(squared))));
}

TEST_CASE("Concat") {
    auto list = concat(List('l', "i"), List("s", 't'));
    REQUIRE(length(list) == 4);

    auto ints = concat(List(1, 2, 3, 4, 5), List(6, 7, 8, 9, 10));
    REQUIRE(accumulate(ints, 500) == 555);

    auto triple = concat(List("Hulk"), concat(List('p', 'q'), List('q', 'p')));
    REQUIRE(length(triple) == 5);

    auto empty = concat(concat(List(), List()), concat(List(), List()));
    REQUIRE(length(empty) == 0);

    auto chaos = concat(concat(List(Empty{}, "Iron Man", 'w', 5ULL), List(CNT, &CreateEmpty, CNT)),
                        concat(List(), concat(List(1, 2), List(List(&StaticCounter::Reset), List(List())))));
    REQUIRE(length(chaos) == 11);
}

// Have no need to compile if lists have different sizes
// Have no need to compile for incomparable types
// Should be implemented without cycles, use C++20
TEST_CASE("Equal") {
    REQUIRE(equal(List(1, 2, 3), List(1, 2, 3)));
    REQUIRE(equal(List(), List()));
    REQUIRE(equal(List(1, 2, 3), List(1.0, 2, 3.0f)));
    REQUIRE(equal(List(std::vector<uint64_t>{1, 2, 3}, 2.0f, Empty{}), List(std::vector<uint64_t>{1, 2, 3}, 2.0f, Empty{})));

    REQUIRE(!equal(List(1, 2, 3), List(2, 4, 6)));
    REQUIRE(!equal(List(STR("aaa")), List(STR("bbb"))));
    REQUIRE(!equal(List(Empty{}, Empty{}, 777), List(Empty{}, Empty{}, 999)));

    SECTION("Head & Tail") {
        auto list = List(1, '2', "third", 4.0f);
        REQUIRE(equal(tail(list), List('2', "third", 4.0f)));
        REQUIRE(equal(tail(tail(list)), List("third", 4.0f)));

        auto third = List(head(tail(tail(list))));
        REQUIRE(equal(List("third", 4.0f), concat(third, List(4.0f))));
    }

    SECTION("For Each") {
        auto bonuses = List("bad-hash", "bad-rand", "clang-fun", "executors",
                            "functional-world", "matrix-2.0", "solve-or-die");
        auto marked_bonuses = for_each(MarkBonus, bonuses);
        REQUIRE(equal(marked_bonuses, List("humdrum-vee", "humdrum-vee", "humdrum-vee", "humdrum-vee", 
                                           "fun-vee", "humdrum-vee", "humdrum-vee")));

        REQUIRE(equal(for_each(CreateEmpty, bonuses), List(Empty{}, Empty{}, Empty{}, Empty{}, Empty{}, Empty{}, Empty{})));
        REQUIRE(equal(for_each([](int a) { return '0' + a; }, List(1, 2)), List('1', '2')));
    }

    SECTION("Concat") {
        REQUIRE(equal(List(1, 2, 3, 4), concat(List(1, 2, 3), List(4))));
        REQUIRE(equal(concat(List(), concat(List(), List())), List()));
        REQUIRE(equal(concat(List('a', "b", "C"), List(9999, 8888, 7777)), List('a', "b", "C", 9999, 8888, 7777)));
    }

    SECTION("Stress") {
        std::mt19937 gen;
        for (size_t i = 0; i < 1000; ++i) {
            auto val1 = gen() % 10, val2 = gen() % 10, val3 = gen() % 10, val4 = gen() % 11;
            REQUIRE(!equal(List(val1, val2, val3, val4), List(10 - val1, 10 - val2, 10 - val3, 11 - val4)));
        }
    }
}

TEST_CASE("Sum Two Lists") {
    auto list = sum_2_lists(List(1, 2, 3), List(10, 20, 30));
    REQUIRE(equal(list, List(11, 22, 33)));

    auto slist = sum_2_lists(List(STR("Anakin "), STR("Obi-Wan ")), List(STR("Skywalker"), STR("Kenobi")));
    REQUIRE(equal(slist, List("Anakin Skywalker", "Obi-Wan Kenobi")));

    auto cnt = sum_2_lists(List(CNT, CNT, CNT), List(CNT, CNT, CNT));
    REQUIRE(counter == 3);

    auto diffs = sum_2_lists(List(1, STR("Darth "), CNT, '4'), List(2, STR("Vader"), CNT, 3));
    auto diffs_sum = List(3, "Darth Vader", CNT, '7');
    REQUIRE(equal(diffs, diffs_sum));

    auto final_stand = sum_2_lists(for_each(Square, List(4, 5, 6, 7, 8, 9)), List(84, 75, 64, 51, 36, 19));
    REQUIRE(equal(List(100, 100, 100, 100, 100, 100), final_stand));
}

TEST_CASE("FlatMap") {
    auto mapped = flatmap(Destroy, List(10000, 9999, -1));
    REQUIRE(equal(mapped, List()));

    auto x10 = flatmap(X10, List('x'));
    REQUIRE(equal(x10, List('x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x')));

    auto randomized = flatmap(PseudoRandomModify, List(8, 4, 9));
    REQUIRE(equal(randomized, List('8', Empty{}, 512, '4', Empty{}, 64, '9', Empty{}, 729)));
}

#define CHECK_FUNC(name) auto func_##name = name

TEST_CASE("You Shall Not PASS") {
    CHECK_FUNC(head);
    CHECK_FUNC(tail);
    CHECK_FUNC(length);
    CHECK_FUNC(accumulate);
    CHECK_FUNC(for_each);
    CHECK_FUNC(concat);
    CHECK_FUNC(sum_2_lists);
    CHECK_FUNC(equal);
    CHECK_FUNC(flatmap); 
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

template <typename T>
auto Identity(T t) { return t; }

auto ToList(int i) { return List(i); }

auto IntToStringList(int i) { return List(std::to_string(i)); }
auto StringToIntList(const std::string& s) { return List(std::stoi(s)); }

auto Wrap(int i) { return List(List(i, 2 * i, 3 * i)); }

template <typename T>
auto TrashTransform(T value) {
    return List(List(value), 11.1111, " you cannot ", List(value, value, value), 
                SomeStruct{1, 77.7f, {"survive this"}}, CNT, List(List(List())), Empty{});
}

#define SuperTrashTransormator TrashTransform<std::any>
#define SuperIdentity Identity<std::any>

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

TEST_CASE("Functional World") {
    {
        auto result = List(1, 2, 3) > Identity<int>;
        REQUIRE(equal(result, List(1, 2, 3)));
    }
    {
        auto result = List(1, 2, 3) >= ToList > Identity<int>;
        REQUIRE(equal(result, List(1, 2, 3)));
    }
    {
        auto result = List(1, 2, 3, 4, 5) >= IntToStringList > Identity<std::string>;
        REQUIRE(equal(result, List("1", "2", "3", "4", "5")));
    }
    {
        auto result = List(1, 2, 3) >= IntToStringList >= StringToIntList > Square;
        REQUIRE(equal(result, List(1, 4, 9)));
    }
    {
        auto result = List(1, 2, 3) >= Wrap >= tail > Square;
        REQUIRE(equal(result, List(4, 9, 16, 36, 36, 81)));
    }
}

TEST_CASE("Functional World Horror") {
    {
        auto result = List(10, "20") >= SuperTrashTransormator >= SuperTrashTransormator >= SuperTrashTransormator > SuperIdentity;
        REQUIRE(length(result) == 1024);
    }
}
