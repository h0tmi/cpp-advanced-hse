#include "string-splitter.h"

#include <string>
#include <vector>
#include <algorithm>

#include "../third-party/some-huge-library.hpp"

class StringSplitterImpl {
public:
    void TakeFirstN(size_t n) {
        take_first_n = n;
    }

    void AddDelimiter(char delimiter) {
        delimiters_.push_back(delimiter);
    }

    std::vector<std::string> Split(const std::string& s) {
        std::vector<std::string> out;

        splitter_.DoSplit(out, s, [this](char x) {
            return std::any_of(delimiters_.begin(), delimiters_.end(),
                               [x](char d) { return d == x; });
        });

        if (take_first_n && out.size() > take_first_n) {
            out.resize(take_first_n);
        }

        return out;
    }

private:
    std::vector<char> delimiters_{};

    size_t take_first_n{};

    InnerStringSplitter splitter_;
};

StringSplitter::StringSplitter() {
    new (&storage_) StringSplitterImpl();
}

StringSplitterImpl* StringSplitter::PImpl() {
    return reinterpret_cast<StringSplitterImpl*>(&storage_);
}

StringSplitter& StringSplitter::TakeFirstN(size_t n) {
    PImpl()->TakeFirstN(n);
    return *this;
}

StringSplitter& StringSplitter::AddDelimiter(char delimiter) {
    PImpl()->AddDelimiter(delimiter);
    return *this;
}

std::vector<std::string> StringSplitter::Split(const std::string& s) {
    return PImpl()->Split(s);
}

StringSplitter::~StringSplitter() {
    PImpl()->~StringSplitterImpl();

    static_assert(sizeof(StringSplitterImpl) == kImplSize);
    static_assert(alignof(StringSplitterImpl) == kImplAlign);
}
