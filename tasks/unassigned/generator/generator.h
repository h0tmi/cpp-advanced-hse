#pragma once

#include <functional>

template <typename T>
class Yield {
public:
    template <typename U>
    void yield(U&& u)
    {
    }
};

template <typename T>
class Generator : public Yield<T> {
public:
    Generator(std::function<void(Yield<T>&)> f)
    {
    }

    void yield(const T& t)
    {
    }

    const T* get()
    {
    }

private:
};
