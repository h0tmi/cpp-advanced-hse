#include "string-splitter.h"

#include <vector>
#include <algorithm>

StringSplitter& StringSplitter::TakeFirstN(size_t n) {
    take_first_n = n;
    return *this;
}

StringSplitter& StringSplitter::AddDelimiter(char delimiter) {
    delimiters_.push_back(delimiter);
    return *this;
}

std::vector<std::string> StringSplitter::Split(const std::string& s) {
    std::vector<std::string> out;

    splitter_.DoSplit(out, s, [this](char x) {
        return std::any_of(delimiters_.begin(), delimiters_.end(), [x](char d) { return d == x; });
    });

    if (take_first_n && out.size() > take_first_n) {
        out.resize(take_first_n);
    }

    return out;
}
