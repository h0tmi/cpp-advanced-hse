#pragma once

template<class K, class V, int MaxSize = 8>
class ConstexprMap {
public:
    constexpr ConstexprMap() = default;

    constexpr V& operator[](const K& key) {
    }

    constexpr const V& operator[](const K& key) const {
    }

    constexpr bool Erase(const K& key) {
    }

    constexpr bool Find(const K& key) const {
    }

    constexpr size_t Size() const {
    }

    constexpr std::pair<K, V>& GetByIndex(size_t pos) {
    }

    constexpr const std::pair<K, V>& GetByIndex(size_t pos) const {
    }
};
