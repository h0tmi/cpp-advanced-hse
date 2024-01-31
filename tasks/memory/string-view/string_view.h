#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <memory>
#include <string>

class StringView {
public:
    StringView(const std::string& s, size_t pos = 0, size_t size = std::string::npos)
        : size_((size == std::string::npos ? s.size() - pos : std::min(s.size() - pos, size))),
          begin_(&s[pos]){};

    StringView(const char* s) : size_(strlen(s)), begin_(s){};
    StringView(const char* s, size_t size) : size_(size), begin_(s){};

    char operator[](size_t pos) const {
        return *(begin_ + pos);
    };
    size_t Size() const {
        return size_;
    };

private:
    size_t size_;
    const char* begin_;
};
