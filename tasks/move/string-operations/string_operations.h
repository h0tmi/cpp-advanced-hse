#pragma once

#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

bool StartsWith(std::string_view string, std::string_view text);

bool EndsWith(std::string_view string, std::string_view text);

std::string_view StripPrefix(std::string_view string, std::string_view prefix);

std::string_view StripSuffix(std::string_view string, std::string_view suffix);

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n = std::string_view::npos);

std::string_view StripAsciiWhitespace(std::string_view s);

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim);

std::string ReadN(const std::string& filename, size_t n);

std::string AddSlash(std::string_view path);

std::string_view RemoveSlash(std::string_view path);

std::string_view Dirname(std::string_view path);

std::string_view Basename(std::string_view path);

std::string CollapseSlashes(std::string_view path);

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter);

template <typename T, typename... Args>
void CoolSizer(size_t& pos, T& cur, Args&... args) {
    if constexpr (std::is_arithmetic_v<T>) {

        T cpm = cur;
        if (cpm < 0) {
            ++pos;
        }

        do {
            ++pos;
            cpm /= 10;
        } while (cpm);
    } else {
        if constexpr (std::is_same<std::decay_t<T>, std::string>::value ||
                      std::is_same<std::decay_t<T>, std::string_view>::value) {
            pos += cur.size();
        } else {
            pos += std::strlen(cur);
        }
    }
}

template <typename T, typename... Args>
void CoolString(std::string& s, T& cur, Args&... args) {
    if constexpr (std::is_arithmetic_v<T>) {
        size_t num_len = 1;

        if (cur < 0) {
            int64_t divider = 1;
            s.push_back('-');
            while (cur / divider < -9LL) {
                divider *= 10;
                ++num_len;
            }
            for (size_t i = 0; i < num_len; ++i) {
                s.push_back(char('0' + cur / divider * -1));
                cur %= divider;
                divider /= 10;
            }
        } else {
            size_t divider = 1;
            while (cur / divider > 9LL) {
                divider *= 10LL;
                ++num_len;
            }
            for (size_t i = 0; i < num_len; ++i) {
                s.push_back(char('0' + cur / divider));
                cur %= divider;
                divider /= 10;
            }
        }

    } else {
        if constexpr (std::is_same<std::decay_t<T>, std::string>::value ||
                      std::is_same<std::decay_t<T>, std::string_view>::value) {
            s += cur;
        } else {
            s += cur;
        }
    }
}

template <typename... Args>
std::string StrCat(Args&&... args) {
    size_t pos = 0;
    (CoolSizer(pos, args), ...);
    std::string ans;
    ans.reserve(pos);
    (CoolString(ans, args), ...);
    return ans;
};
