#include <catch.hpp>
#include <test.h>
#include <stdexcept>
#include <vector>
#include <iostream>

enum class Method { kCreate, kSetup, kRun, kTeardown };

struct Event {
    Method method;
    std::string test;
    Event() {
    }
    Event(Method method, const std::string& test) : method(method), test(test) {
    }

    bool operator==(const Event& rhs) const {
        return method == rhs.method && test == rhs.test;
    }
};

std::ostream& operator<<(std::ostream& out, const Event& event) {
    switch (event.method) {
        case Method::kCreate:
            out << "create";
            break;
        case Method::kSetup:
            out << "setup";
            break;
        case Method::kRun:
            out << "run";
            break;
        case Method::kTeardown:
            out << "teardown";
            break;
    }
    out << " " << event.test;
    return out;
}

std::vector<Event> events;

class SimpleTest : public AbstractTest {
public:
    virtual void SetUp() override {
        events.emplace_back(Method::kSetup, "simple_test");
    }
    virtual void TearDown() override {
        events.emplace_back(Method::kTeardown, "simple_test");
    }
};

class FirstTest : public SimpleTest {
public:
    FirstTest() {
        events.emplace_back(Method::kCreate, "first_test");
    }
    virtual void Run() override {
        events.emplace_back(Method::kRun, "first_test");
    }
};

class SecondTest : public SimpleTest {
public:
    SecondTest() {
        events.emplace_back(Method::kCreate, "second_test");
    }

    SecondTest(const SecondTest&) = delete;
    SecondTest(SecondTest&&) = delete;
    SecondTest& operator=(const SecondTest&) = delete;
    SecondTest& operator=(SecondTest&&) = delete;

    virtual void Run() override {
        events.emplace_back(Method::kRun, "second_test");
    }

    virtual void TearDown() override {
        events.emplace_back(Method::kTeardown, "second_test");
    }
};

class ThirdTest : public AbstractTest {
    virtual void Run() override {
        throw std::runtime_error("shit happens");
    }

    virtual void SetUp() override {
        events.emplace_back(Method::kSetup, "third_test");
    }

    virtual void TearDown() override {
        events.emplace_back(Method::kTeardown, "third_test");
    }
};

void Reset() {
    events.clear();
    auto& instance = TestRegistry::Instance();
    instance.Clear();
}

TEST_CASE("Registry is singleton", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<FirstTest>("first_test");
    registry.RegisterClass<SecondTest>("second_test");
    {
        auto& new_registry = TestRegistry::Instance();
        std::vector<std::string> expected{"first_test", "second_test"};
        REQUIRE(expected == new_registry.ShowAllTests());
    }
}

TEST_CASE("Registry doesn't run tests", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<SecondTest>("check");
    std::vector<std::string> expected{"check"};
    REQUIRE(expected == registry.ShowAllTests());
    REQUIRE(events.empty());
}

TEST_CASE("Show filter of registry", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<FirstTest>("aba");
    registry.RegisterClass<FirstTest>("hello");
    registry.RegisterClass<FirstTest>("caba");
    {
        std::vector<std::string> expected{"aba", "caba"};
        REQUIRE(expected == registry.ShowTests(Substr("aba")));
        expected.pop_back();
        REQUIRE(expected == registry.ShowTests(FullMatch("aba")));
    }
    {
        std::vector<std::string> expected;
        REQUIRE(expected == registry.ShowTests(FullMatch("hell")));
    }
}

TEST_CASE("Single run", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<FirstTest>("first_test");
    registry.RunTest("first_test");
    std::vector<Event> expected{{Method::kCreate, "first_test"},
                                {Method::kSetup, "simple_test"},
                                {Method::kRun, "first_test"},
                                {Method::kTeardown, "simple_test"}};
    REQUIRE(expected == events);
}

int counter = 0;
class AnotherTest : public SimpleTest {
public:
    AnotherTest() {
        counter = 0;
    }

    virtual void Run() override {
        ++counter;
    }
};

TEST_CASE("Create tests", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<SecondTest>("second_test");
    registry.RegisterClass<ThirdTest>("third_test");
    registry.RegisterClass<AnotherTest>("check");
    auto test = registry.CreateTest("third_test");
    test->SetUp();
    try {
        test->Run();
        REQUIRE(false);
    } catch (const std::exception&) {
        // ok
    }

    try {
        registry.CreateTest("first_test");
        REQUIRE(false);
    } catch (const std::out_of_range&) {
        // ok
    }

    auto first_test = registry.CreateTest("check");
    first_test->Run();
    REQUIRE(counter == 1);
    auto second_test = registry.CreateTest("check");
    second_test->Run();
    REQUIRE(counter == 1);
}

TEST_CASE("Run all", "[test]") {
    Reset();
    auto& registry = TestRegistry::Instance();
    registry.RegisterClass<ThirdTest>("third_test");
    registry.RegisterClass<FirstTest>("first_test");
    registry.RegisterClass<SecondTest>("second_test");
    try {
        registry.RunTests([](const std::string&) { return true; });
        REQUIRE(false);
    } catch (const std::exception& e) {
        REQUIRE(std::string(e.what()) == "shit happens");
    }
    std::vector<Event> expected{
        {Method::kCreate, "first_test"},  {Method::kSetup, "simple_test"},
        {Method::kRun, "first_test"},     {Method::kTeardown, "simple_test"},
        {Method::kCreate, "second_test"}, {Method::kSetup, "simple_test"},
        {Method::kRun, "second_test"},    {Method::kTeardown, "second_test"},
        {Method::kSetup, "third_test"},   {Method::kTeardown, "third_test"}};
    REQUIRE(expected == events);
}
