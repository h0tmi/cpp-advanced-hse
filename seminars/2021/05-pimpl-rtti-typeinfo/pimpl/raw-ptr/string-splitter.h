#pragma once

#include <cstddef>
#include <vector>
#include <string>

class StringSplitterImpl;

class StringSplitter {
public:
    StringSplitter();

    StringSplitter& TakeFirstN(size_t n);

    StringSplitter& AddDelimiter(char delimiter);

    std::vector<std::string> Split(const std::string& s);

    ~StringSplitter();

private:
    StringSplitterImpl* p_impl;
};
