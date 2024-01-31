#pragma once

#include <algorithm>
#include <cinttypes>
#include <functional>
#include <memory>
#include <map>
#include <stdexcept>
#include <string>

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() {
    }
};

class FullMatch {
public:
    FullMatch(const std::string& test_name) : test_name_(test_name) {
    }

    bool operator()(const std::string& test_name) {
        return test_name == test_name_;
    }

private:
    std::string test_name_;
};

class Substr {
public:
    Substr(const std::string& test_name) : test_name_(test_name) {
    }

    bool operator()(const std::string& test_name) {
        return test_name.find(test_name_) != std::string::npos;
    }

private:
    std::string test_name_;
};

class TestRegistry {
public:
    // Singletone
    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        tests_list_[class_name] = []() -> std::unique_ptr<AbstractTest> {
            return std::make_unique<TestClass>();
        };
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        if (tests_list_.contains(class_name)) {
            return tests_list_[class_name]();
        }
        throw std::out_of_range("No test with this name");
    }

    void RunTest(const std::string& test_name) {
        const auto test = CreateTest(test_name);
        try {
            test->SetUp();
            test->Run();
            test->TearDown();
        } catch (...) {
            test->TearDown();
            throw;
        }
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> test_names;
        for (const auto& test : tests_list_) {
            if (callback(test.first)) {
                test_names.emplace_back(test.first);
            }
        }
        std::sort(test_names.begin(), test_names.end());
        return test_names;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> test_names;
        for (const auto& test : tests_list_) {
            test_names.emplace_back(test.first);
        }
        std::sort(test_names.begin(), test_names.end());
        return test_names;
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        const auto tests = ShowTests(std::forward<Predicate>(callback));
        for (const auto& test : tests) {
            RunTest(test);
        }
    }

    void Clear() {
        tests_list_.clear();
    }

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<AbstractTest>()>> tests_list_;
};
