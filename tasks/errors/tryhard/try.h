#pragma once

#include <exception>
#include <stdexcept>

template <class T>
class Try;

template <>
class Try<void>;

template <class Function, class... Args>
auto TryRun(Function func, Args... args) {
    using ReturnType = decltype(func(args...));
}
