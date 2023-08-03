#pragma once

#include <any>
#include <utility>

class SimpleAny {
public:
    template <typename UniversalT>
    SimpleAny(UniversalT&& value) : value_{std::forward<UniversalT>(value)} {
    }

    template <typename T>
    auto CastUnsafe() -> T {
        return std::any_cast<T>(value_);
    }

private:
    std::any value_;
};
