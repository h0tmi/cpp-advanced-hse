#include "function_ref.h"

#include <allocations_checker.h>
#include <catch.hpp>

#include <algorithm>
#include <vector>
#include <string_view>


template <typename Signature>
struct TTestFunction;

template <typename Ret, typename... Args>
struct TTestFunction<Ret(Args...)> {
    Ret operator()(Args...) const {
        return {};
    }
};

TEST_CASE("Default constructible") {
    static_assert(!std::is_default_constructible_v<FunctionRef<void()>>);
    static_assert(!std::is_default_constructible_v<FunctionRef<int(double, void********* megaptr, TTestFunction<void(int)>)>>);
}

TEST_CASE("Sizeof") {
    static_assert(sizeof(FunctionRef<int(int)>) == sizeof(void*) * 2);
}

void Iterate(int from, int to, FunctionRef<void(int)> callback) {
    while (from < to) {
        callback(from++);
    }
}

TEST_CASE("AsArgument") {
    int sum = 0;
    Iterate(0, 10, [&](int x) { sum += x; });
    REQUIRE(sum == 45);

    Iterate(0, 10, [&](int x) noexcept { sum += x; });
    REQUIRE(sum == 90);

    auto summer = [&](int x) { sum += x; };
    Iterate(0, 10, summer);
    Iterate(0, 10, summer);
    Iterate(0, 10, summer);
    REQUIRE(sum == 225);

    FunctionRef<void(int)> ref = summer;
    Iterate(0, 10, ref);
    REQUIRE(sum == 270);
}

TEST_CASE("Allocations") {
    EXPECT_ZERO_ALLOCATIONS(
        int sum = 0;
        Iterate(0, 10, [&](int x) { sum += x; });
        REQUIRE(sum == 45);

        Iterate(0, 10, [&](int x) noexcept { sum += x; });
        REQUIRE(sum == 90);

        auto summer = [&](int x) { sum += x; };
        Iterate(0, 10, summer);
        Iterate(0, 10, summer);
        Iterate(0, 10, summer);
        REQUIRE(sum == 225);

        FunctionRef<void(int)> ref = summer;
        Iterate(0, 10, ref);
        REQUIRE(sum == 270);
    );
}

int global_sum = 0;
void AddToGlobalSum(int x) {
    global_sum += x;
}

TEST_CASE("FunctionPointer") {
    global_sum = 0;
    Iterate(0, 10, AddToGlobalSum);
    REQUIRE(global_sum == 45);

    FunctionRef<void(int)> ref1 = AddToGlobalSum;
    Iterate(0, 10, ref1);
    REQUIRE(global_sum == 90);

    FunctionRef<void(int)> ref2{AddToGlobalSum};
    Iterate(0, 10, ref2);
    REQUIRE(global_sum == 135);

    FunctionRef<void(int)> ref3 = &AddToGlobalSum;
    Iterate(0, 10, ref3);
    REQUIRE(global_sum == 180);
}

TEST_CASE("Reassign") {
    FunctionRef<void(double)> kek = [](double) { return 42; };
    kek = [](double) { return 19; };
    kek = [](int) { return 22.8; };
}

const char* Greet() {
    return "Hello, world!";
}

TEST_CASE("ImplicitCasts") {
    FunctionRef<void(int)> ref = [](int x) { return x; };
    ref = [](double x) { return x; };
    ref = [](char x) { return x; };

    FunctionRef<int()> ref1 = [] { return 0.5; };
    ref1 = [] { return 'a'; };
    ref1 = [] { return 124u; };

    FunctionRef<std::string_view()> ref2{Greet};
}

TEST_CASE("StatelessLambdaLifetime") {
    FunctionRef<int(int, int)> ref{[](int a, int b) { return a + b; }};
    REQUIRE(ref(5, 5) == 10);
}

TEST_CASE("ForwardArguments") {
    char x = 'x';
    FunctionRef<void(std::unique_ptr<int>, char&)> ref = [](std::unique_ptr<int> ptr, char& ch) {
        REQUIRE(*ptr == 5);
        ch = 'a';
    };
    ref(std::make_unique<int>(5), x);
    REQUIRE(x == 'a');
}

auto MakeLambda(int x) {
    return [x] {
        return x;
    };
}

TEST_CASE("AntiStatic-1") {
    auto l1 = MakeLambda(1);
    FunctionRef<int()> r1(l1);
    REQUIRE(r1() == 1);

    auto l2 = MakeLambda(2);
    FunctionRef<int()> r2(l2);
    REQUIRE(r2() == 2);

    REQUIRE(r1() == 1);
    REQUIRE(r2() == 2);
}

template<int N>
int F() {
    return N;
}

TEST_CASE("AntiStatic-2") {
    FunctionRef<int()> r1(F<1>);
    REQUIRE(r1() == 1);

    FunctionRef<int()> r2(F<2>);
    REQUIRE(r2() == 2);

    REQUIRE(r1() == 1);
    REQUIRE(r2() == 2);
}
