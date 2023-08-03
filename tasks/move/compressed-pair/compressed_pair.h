#pragma once

#include <type_traits>

// Me think, why waste time write lot code, when few code do trick.
template <typename F, typename S>
class CompressedPair {
public:
    CompressedPair(const F& first, const S& second) : first_(first), second_(second) {
    }

    F& GetFirst() {
        return first_;
    }

    const S& GetSecond() const {
        return second_;
    };

private:
    F first_;
    S second_;
};