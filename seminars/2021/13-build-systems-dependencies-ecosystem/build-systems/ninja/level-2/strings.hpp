#pragma once

#include <string>

namespace strings {

// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring

// trim from start (in place)
void LeftTrim(std::string &s);

// trim from end (in place)
void RightTrim(std::string &s);

// trim from both ends (in place)
void Trim(std::string &s);

// trim from start (copying)
std::string LeftTrimCopy(std::string s);

// trim from end (copying)
std::string RightTrimCopy(std::string s);

// trim from both ends (copying)
std::string TrimCopy(std::string s);

}
