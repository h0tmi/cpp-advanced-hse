#include "harakiri.h"

#include <catch.hpp>
#include <string>
#include <type_traits>
#include <utility>

class DoConstWork {
public:
    std::string Do(const OneTimeCallback* callback) {
        // Pretend to do some work.
        return std::move(*callback)();
        // Try this and make sure it fails.
        // return std::move(*callback)() + std::move(*callback)();
        // return (*callback)();
    }
};

class DoWork {
public:
    std::string Do(OneTimeCallback* callback) {
        // Pretend to do some work.
        return std::move(*callback)();
        // Try this and make sure it fails.
        // return std::move(*callback)() + std::move(*callback)();
        // return (*callback)();
    }
};

TEST_CASE("Test simple calls") {
    static_assert(!std::is_invocable_v<decltype(&AwesomeCallback::operator()), AwesomeCallback&>,
                  "Think more");
    static_assert(
        !std::is_invocable_v<decltype(&AwesomeCallback::operator()), const AwesomeCallback&>,
        "Think more");
    static_assert(std::is_invocable_v<decltype(&AwesomeCallback::operator()), AwesomeCallback&&>,
                  "Think more");
    static_assert(
        std::is_invocable_v<decltype(&AwesomeCallback::operator()), const AwesomeCallback&&>,
        "Think more");
}

TEST_CASE("Test Const Callback") {
    const AwesomeCallback* c = new AwesomeCallback("pony shines extreme ");
    DoConstWork d;
    REQUIRE(d.Do(c) == "pony shines extreme awesomeness");
}

TEST_CASE("Test Callback") {
    AwesomeCallback* c = new AwesomeCallback(std::string("C++ shine extreme "));
    DoWork d;
    REQUIRE(d.Do(c) == "C++ shine extreme awesomeness");
}
