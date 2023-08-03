#pragma once

#include <boost/algorithm/string.hpp>

// Just to force include this header to string-splitter.h
class InnerStringSplitter {
public:
    template <typename F>
    void DoSplit(std::vector<std::string>& out, const std::string& s, F&& is_delim) {
        boost::split(out, s, is_delim);
    }
};
