#pragma once

#include <functional>

class Coroutine {
public:
    explicit Coroutine(std::function<void()> f)
    {
    }

    void resume()
    {
    }

    void suspend()
    {
    }

private:
};

void suspend()
{
}
