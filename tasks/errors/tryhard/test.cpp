#include <catch.hpp>

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <cstdio>
#include <type_traits>

#include <try.h>

TEST_CASE("Try has constructors", "[Try]") {
    Try<int> a;
    Try<int> b(5);
    Try<std::string> c(std::runtime_error("err"));

    REQUIRE(b.Value() == 5);
}

TEST_CASE("Try has state", "[Try]") {
    Try<double> a(std::logic_error("err"));
    REQUIRE(a.IsFailed() == true);
    Try<int> b;
    REQUIRE(b.IsFailed() == false);
    Try<std::vector<int>> c(std::vector<int>{1, 2, 3});
    REQUIRE(c.IsFailed() == false);
    try {
        a.Value();
        REQUIRE(false);
    } catch (const std::exception& e) {
        REQUIRE(std::string(e.what()) == "err");
        // OK
    }

    try {
        b.Value();
        REQUIRE(false);
    } catch (const std::exception& e) {
        REQUIRE(std::string(e.what()) == "Object is empty");
    }

    try {
        b.Throw();
        REQUIRE(false);
    } catch (const std::exception& e) {
        REQUIRE(std::string(e.what()) == "No exception");
    }

    try {
        c.Throw();
        REQUIRE(false);
    } catch (const std::exception& e) {
        REQUIRE(std::string(e.what()) == "No exception");
    }

    std::vector<int> expected{1, 2, 3};
    REQUIRE(c.Value() == expected);
}

template <class Type, class Check = decltype(std::declval<Type>().Value())>
void CheckValueExistence(Type) {
    REQUIRE(std::string("Your void has Value()") == "false");  // good luck with this error msg
}

template <class Type, class... Args>
void CheckValueExistence(Type, Args...) {
    // OK
}

TEST_CASE("Try<void> is really void", "[Try]") {
    Try<void> test;
    CheckValueExistence(test);
}

TEST_CASE("Try doesn't use public inheritance", "[Try]") {
    static_assert(!std::is_convertible<Try<void>*, Try<int>*>::value, "Inheritance test 1");
    static_assert(!std::is_convertible<Try<int>*, Try<void>*>::value, "Inheritance test 2");
}

TEST_CASE("TryRun wrapper works properly", "[try]") {
    auto f1 = []() -> void { throw std::invalid_argument("invalid"); };
    auto f2 = [](const std::string& s) { return s; };
    auto f3 = [](int left, int right) {
        if (left < right) {
            return left;
        }
        throw "Shit happens";
    };
    auto f4 = [](const std::vector<char>& data, size_t pos) { return data.at(pos); };
    auto f5 = [](const char* /*filename*/, const char* /*mode*/, int /*flags*/) -> void {
        throw ENOENT;
    };

    auto f6 = [](std::vector<int>& elems, int x) {
        elems.push_back(x);
        if (elems.size() == 3u) {
            throw elems;  // wtf
        }
    };

    {
        Try<void> a1 = TryRun(f1);
        REQUIRE(a1.IsFailed() == true);
        try {
            a1.Throw();
        } catch (const std::exception& e) {
            // ok
        }
    }

    {
        Try<std::string> a2 = TryRun(f2, "hello!");
        REQUIRE(a2.IsFailed() == false);
        REQUIRE(a2.Value() == "hello!");
    }

    {
        Try<int> a31 = TryRun(f3, 3, 5);
        REQUIRE(a31.IsFailed() == false);
        REQUIRE(a31.Value() == 3);
    }

    {
        Try<int> a32 = TryRun(f3, 5, 3);
        REQUIRE(a32.IsFailed() == true);
        try {
            a32.Value();
        } catch (const std::exception& e) {
            REQUIRE(std::string("Shit happens") == e.what());
        }
    }

    std::vector<char> data{'a', 'b', 'c'};
    {
        Try<char> a41 = TryRun(f4, data, 0u);
        REQUIRE(a41.IsFailed() == false);
        REQUIRE(a41.Value() == 'a');
    }

    {
        Try<char> a42 = TryRun(f4, data, 3u);
        REQUIRE(a42.IsFailed() == true);
        try {
            a42.Throw();
        } catch (const std::exception& e) {
            // OK
        }
    }

    {
        Try<void> a5 = TryRun(f5, "test.cpp", "r", 0);
        REQUIRE(a5.IsFailed() == true);
        try {
            a5.Throw();
        } catch (const std::exception& e) {
            REQUIRE(std::string("No such file or directory") == e.what());
        }
    }

    {
        std::vector<int> elems;
        Try<void> a6 = TryRun(
            [f6, &elems](int count) {
                for (int i = 0; i < count; ++i) {
                    f6(elems, i);
                }
            },
            4);

        REQUIRE(a6.IsFailed() == true);
        try {
            a6.Throw();
        } catch (const std::exception& e) {
            REQUIRE(std::string("Unknown exception") == e.what());
        }
    }
}

TEST_CASE("Try saves object", "[Try]") {
    class MySuperException : public std::exception {
    public:
        virtual const char* what() const noexcept override {
            return "Check";
        }
    };

    const std::exception* addr;
    auto f = [&addr](int) {
        try {
            throw MySuperException();
        } catch (const std::exception& e) {
            addr = &e;
            throw;
        }
    };

    auto result = TryRun(f, 5);
    try {
        result.Throw();
    } catch (const std::exception& e) {
        REQUIRE(&e == addr);
    }
}
