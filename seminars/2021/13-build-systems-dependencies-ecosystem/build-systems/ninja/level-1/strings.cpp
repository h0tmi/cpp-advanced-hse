#include "strings.hpp"

#include <algorithm>
#include <cctype>

namespace strings {

void LeftTrim(std::string &s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void RightTrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

void Trim(std::string &s) {
    LeftTrim(s);
    RightTrim(s);
}

std::string LeftTrimCopy(std::string s) {
    LeftTrim(s);
    return s;
}

std::string RightTrimCopy(std::string s) {
    RightTrim(s);
    return s;
}

std::string TrimCopy(std::string s) {
    Trim(s);
    return s;
}

}  // namespace strings
