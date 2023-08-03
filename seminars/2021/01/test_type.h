#pragma once

#include <string>
#include <iostream>

struct Test {
    Test() = default;
    explicit Test(std::string me) : me_(std::move(me)) {
        std::cout << "Test(" << me_ << ")\n";
    }

    ~Test() {
        std::cout << "~Test(" << me_ << ")\n";
    }

    Test(const Test&) = default;
    Test(Test&&) = default;
    Test& operator=(const Test&) = default;
    Test& operator=(Test&&) = default;

    std::string Get() const {
        return me_;
    }

private:
    std::string me_;
};