#pragma once

#include <string>

// Should not allow reuse and yell under sanitizers.
// Fix the interface and implementation.
// AwesomeCallback should add "awesomeness".

class OneTimeCallback {
public:
    virtual ~OneTimeCallback() = default;
    virtual std::string operator()() = 0;
};

// Implement ctor, operator(), maybe something else...
class AwesomeCallback : public OneTimeCallback {};
