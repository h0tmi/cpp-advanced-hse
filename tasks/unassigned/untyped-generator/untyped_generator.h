#pragma once

#include <functional>

#include <boost/any.hpp>

template <typename T>
void yield(T&& t)
{

}

class GeneratorUntyped {
public:
    GeneratorUntyped(std::function<void()> f) {}

    boost::any get()
    {
    }
};
