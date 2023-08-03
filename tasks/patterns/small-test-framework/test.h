#pragma once

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() {
    }
};

class TestRegistry {
public:
    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
    }

    void RunTest(const std::string& test_name) {
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
    }

    std::vector<std::string> ShowAllTests() const {
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
    }

    void Clear() {
    }
};
