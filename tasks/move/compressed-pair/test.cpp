#include "compressed_pair.h"

#include <catch.hpp>
#include <iostream>
#include <type_traits>
#include <memory>
#include <utility>
#include <mutex>

struct Empty {};

struct NonEmpty {
    int field = 0;
};

struct MoveOnly {
    MoveOnly() = default;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&& other) = default;
};

struct NonEmptyMoveOnly : public MoveOnly, public NonEmpty {};

struct CopyOnly {
    CopyOnly() = default;
    CopyOnly(const CopyOnly&) = default;
    CopyOnly& operator=(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&) = delete;
    CopyOnly& operator=(CopyOnly&&) = delete;
};

struct NonEmptyCopyOnly : public CopyOnly, NonEmpty {};

struct EmptyButFinal final {};

TEST_CASE("NonCompressedSimple") {
    using TestType = CompressedPair<int, std::string>;
    TestType p{1, "hello"};
    REQUIRE(p.GetFirst() == 1);
    REQUIRE(p.GetSecond() == "hello");
}

TEST_CASE("NonCompressedModificationWorks") {
    CompressedPair<int, std::string> p{1, "hello"};
    p.GetFirst() = 5;
    p.GetSecond()[3] = 'k';
    REQUIRE(p.GetFirst() == 5);
    REQUIRE(p.GetSecond() == "helko");
}

TEST_CASE("SimpleOperationsWithCompressedPair") {
    CompressedPair<std::string, std::allocator<std::string>> p = {"hello",
                                                                  std::allocator<std::string>{}};
    std::string* ptr = new (p.GetSecond().allocate(1)) std::string{p.GetFirst()};
    REQUIRE(*ptr == "hello");
    ptr->~basic_string<char>();
    p.GetSecond().deallocate(ptr, 1);
}

struct NoDefaultConstructor {
    NoDefaultConstructor() = delete;
    NoDefaultConstructor(int f) : field(f) {
    }

    int field;
};

TEST_CASE("Constructors") {
    // Default.
    CompressedPair<int, std::string> p0;
    REQUIRE(p0.GetFirst() == 0);
    REQUIRE(p0.GetSecond().empty());

    // Copy and move for empty classes.
    CopyOnly copy1;
    MoveOnly move1;
    CompressedPair<CopyOnly, MoveOnly> p1{copy1, std::move(move1)};

    // Copy and move for non-empty classes.
    NonEmptyCopyOnly copy2;
    NonEmptyMoveOnly move2;
    CompressedPair<NonEmptyCopyOnly, NonEmptyMoveOnly> p3{copy2, std::move(move2)};

    CompressedPair<NoDefaultConstructor, int> p4(NoDefaultConstructor{3}, 0);
    REQUIRE(p4.GetFirst().field == 3);
}

template <class V>
inline constexpr bool is_compressed_v = std::is_empty_v<V> && !std::is_final_v<V>;  // NOLINT

template <class U, class V>
void CheckSize() {
    if constexpr (is_compressed_v<U> && is_compressed_v<V>) {
        if constexpr (std::is_base_of_v<U, V> || std::is_base_of_v<V, U>) {
            static_assert(sizeof(CompressedPair<U, V>) == 2);
        } else {
            static_assert(sizeof(CompressedPair<U, V>) == 1);
        }
    } else if constexpr (is_compressed_v<V> && !std::is_base_of_v<V, U>) {
        static_assert(sizeof(CompressedPair<U, V>) == sizeof(U));
    } else if constexpr (is_compressed_v<U> && !std::is_base_of_v<U, V>) {
        static_assert(sizeof(CompressedPair<U, V>) == sizeof(V));
    } else {
        static_assert(sizeof(CompressedPair<U, V>) == sizeof(std::pair<U, V>));
    }
};

struct MyStatelessFunctor {
    std::string operator()() const {
        return "I am so tired.";
    }
};

struct Empty2 {};

struct Empty3 : Empty2 {};

struct NonEmptyDescendant : Empty {
    int field;
};

union EmptyUnion {};

struct Empty4 {};
struct Empty5 {};
struct Empty6 {};

struct EmptyBase {};
struct EmptyDerived1 : EmptyBase {};
struct EmptyDerived2 : EmptyBase {};
struct NonEmptyDerived : NonEmpty, EmptyBase {};

TEST_CASE("Sizes") {
    CheckSize<int, Empty>();
    CheckSize<Empty, long double>();
    CheckSize<std::string, EmptyButFinal>();
    CheckSize<Empty, Empty>();
    CheckSize<std::unique_ptr<std::string>, Empty>();
    CheckSize<Empty, std::mutex>();
    CheckSize<std::basic_string<int>, MyStatelessFunctor>();
    CheckSize<Empty, Empty2>();
    CheckSize<Empty2, Empty3>();
    CheckSize<Empty, NonEmptyDescendant>();
    CheckSize<EmptyUnion, int>();
    static_assert(std::is_empty_v<CompressedPair<Empty4, Empty5>>);
    static_assert(
        sizeof(CompressedPair<CompressedPair<Empty4, Empty5>, CompressedPair<Empty6, char>>) ==
        sizeof(char));
    static_assert(sizeof(CompressedPair<EmptyDerived1, EmptyDerived2>) == 2);
    static_assert(sizeof(CompressedPair<NonEmptyDerived, EmptyDerived1>) == 8);
}

TEST_CASE("ConstMethods") {
    const CompressedPair<int, std::string> p1{1, "hello!"};
    REQUIRE(p1.GetFirst() == 1);
    REQUIRE(p1.GetSecond() == "hello!");

    const CompressedPair<int, MyStatelessFunctor> p2{1, MyStatelessFunctor{}};
    REQUIRE(p2.GetFirst() == 1);
    REQUIRE(p2.GetSecond()() == "I am so tired.");
}
