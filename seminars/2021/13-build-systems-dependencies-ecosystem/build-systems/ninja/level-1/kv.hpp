#pragma once

#include "simple_any.hpp"

#include <string>
#include <unordered_map>

class KVStorage {
public:
    KVStorage() = default;

    template <typename T>
    auto Set(const std::string& key, const T& value) -> void {
        storage_.emplace(key, value);
    }

    template <typename T>
    auto GetUnsafe(const std::string& key) -> T {
        return storage_.at(key).CastUnsafe<T>();
    }

    auto Size() const noexcept -> std::size_t;
    auto IsEmpty() const noexcept -> bool;

private:
    std::unordered_map<std::string, SimpleAny> storage_;
};
