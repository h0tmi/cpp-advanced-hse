#pragma once

#include <algorithm>
#include <memory>
#include <string>

// Should not allow reuse and yell under sanitizers.
// Fix the interface and implementation.
// AwesomeCallback should add "awesomeness".

class OneTimeCallback {
public:
    OneTimeCallback() = default;
    virtual std::string operator()() const&& = 0;
    virtual ~OneTimeCallback() = default;
};

class AwesomeCallback : public OneTimeCallback {
public:
    AwesomeCallback() = default;
    AwesomeCallback(AwesomeCallback&& other);
    AwesomeCallback& operator=(AwesomeCallback&& other);
    AwesomeCallback(const std::string& s);
    std::string operator()() const&& override final;
    ~AwesomeCallback();

private:
    std::string* data_ = nullptr;
};
