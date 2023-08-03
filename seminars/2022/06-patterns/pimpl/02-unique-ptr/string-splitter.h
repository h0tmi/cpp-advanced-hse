#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <memory>

class StringSplitterImpl;

class StringSplitter {
public:
    StringSplitter();

    StringSplitter& TakeFirstN(size_t n);

    StringSplitter& AddDelimiter(char delimiter);

    std::vector<std::string> Split(const std::string& s);

    // ~StringSplitter = default; // does not work, at this point StringSplitterImpl is incomplete
    ~StringSplitter();

private:
    std::unique_ptr<StringSplitterImpl> p_impl;

    // if using shared, no need to define destructor and implement it in .cpp, discuss why
    // std::shared_ptr<StringSplitterImpl> p_impl;
};
