#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <memory>

class StringSplitterImpl;

constexpr size_t kImplSize = 40;
constexpr size_t kImplAlign = 8;

class StringSplitter {
public:
    StringSplitter();

    StringSplitter& TakeFirstN(size_t n);

    StringSplitter& AddDelimiter(char delimiter);

    std::vector<std::string> Split(const std::string& s);

    ~StringSplitter();

private:
    // std::aligned_storage_t<sizeof(StringSplitterImpl), alignof(StringSplitterImpl)> storage_;
    std::aligned_storage_t<kImplSize, kImplAlign> storage_;

    StringSplitterImpl* PImpl();
};
