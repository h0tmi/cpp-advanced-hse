#pragma once

#include <string>
#include <vector>
#include <algorithm>

// need to include to use InnerStringSplitter

#include "../third-party/some-huge-library.hpp"

class StringSplitter {
public:
    StringSplitter& TakeFirstN(size_t n);

    StringSplitter& AddDelimiter(char delimiter);

    std::vector<std::string> Split(const std::string& s);

private:
    std::vector<char> delimiters_{};

    size_t take_first_n{};

    InnerStringSplitter splitter_;
};
