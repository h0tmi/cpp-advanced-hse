#pragma once

#include <algorithm>
#include <cstddef>

class ConstexprString {
public:
    constexpr ConstexprString(): size_(0), data_(nullptr) {}

    template<size_t N>
    constexpr ConstexprString(const char (&str)[N]): size_(N - 1), data_(&str[0]) {}

    constexpr size_t Size() const {
        return size_;
    }

    constexpr bool operator==(const ConstexprString& rhs) const {
        return Compare(rhs) == 0;
    }

    constexpr bool operator<(const ConstexprString& rhs) const {
        return Compare(rhs) < 0;
    }
private:
    size_t size_;
    const char *data_;

    constexpr int Compare(const ConstexprString& rhs) const {
        for (size_t i = 0; i < std::min(size_, rhs.size_); ++i) {
            if (data_[i] < rhs.data_[i]) {
                return -1;
            } else if (data_[i] > rhs.data_[i]) {
                return 1;
            }
        }
        if (size_ < rhs.size_) {
            return -1;
        }
        if (size_ > rhs.size_) {
            return 1;
        }
        return 0;
    }
};
