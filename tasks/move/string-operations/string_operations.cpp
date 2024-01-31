#include "string_operations.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cctype>
#include <cstddef>
#include <string_view>
#include <fcntl.h>
#include <variant>
#include <vector>
#include <cmath>

bool StartsWith(std::string_view string, std::string_view text) {
    if (text.size() > string.size()) {
        return false;
    }

    size_t i = 0, j = 0;

    while (i < string.size() && j < text.size()) {
        if (string[i] != text[j]) {
            return false;
        }
        ++i;
        ++j;
    }

    return true;
};

bool EndsWith(std::string_view string, std::string_view text) {
    if (text.size() > string.size()) {
        return false;
    }

    size_t i = string.size();
    size_t j = text.size();

    while (i != 0 && j != 0) {
        --i;
        --j;
        if (string[i] != text[j]) {
            return false;
        }
    }

    return true;
}

std::string_view StripPrefix(std::string_view string, std::string_view prefix) {
    if (!StartsWith(string, prefix)) {
        return string;
    }
    return string.substr(prefix.size());
};

std::string_view StripSuffix(std::string_view string, std::string_view suffix) {
    if (!EndsWith(string, suffix)) {
        return string;
    }
    return string.substr(0, string.size() - suffix.size());
};

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n) {
    if (pos >= s.size() || n > s.size()) {
        return s;
    }

    return s.substr(pos, n);
}

std::string_view StripAsciiWhitespace(std::string_view s) {
    if (s.empty()) {
        return s;
    }
    size_t pref_pos = 0;
    size_t suff_pos = s.size() - 1;

    while (pref_pos < suff_pos && std::isspace(s[pref_pos])) {
        ++pref_pos;
    }

    while (suff_pos >= pref_pos && std::isspace(s[suff_pos])) {
        --suff_pos;
    }

    return s.substr(pref_pos, suff_pos - pref_pos + 1);
};

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim) {
    std::vector<std::string_view> splitted_strings(std::max(text.size(), static_cast<size_t>(1)));

    size_t current_id = 0;
    size_t start_pos = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        if (StartsWith(text.substr(i), delim)) {
            splitted_strings[current_id++] = text.substr(start_pos, i - start_pos);
            start_pos = i + delim.size();
        }
        if (i + 1 == text.size()) {
            splitted_strings[current_id++] = text.substr(start_pos, text.size() - start_pos);
        }
    }
    splitted_strings.resize(std::max(current_id, static_cast<size_t>(1)));
    return splitted_strings;
};

std::string ReadN(const std::string& filename, size_t n) {
    int descriptor = open(filename.data(), O_RDONLY);
    if (descriptor == -1 || n == 0) {
        return std::string();
    }

    std::string result(n, '0');

    read(descriptor, result.data(), n);

    return result;
};

std::string AddSlash(std::string_view path) {
    if (path.back() == '/') {
        return static_cast<std::string>(path);
    }
    std::string result(path.size() + 1, '/');
    for (size_t i = 0; i < path.size(); ++i) {
        result[i] = path[i];
    }
    return result;
};

std::string_view RemoveSlash(std::string_view path) {
    if (path.size() != 1 && path.back() == '/') {
        return path.substr(0, path.size() - 1);
    }
    return path;
};

std::string_view Dirname(std::string_view path) {
    auto pos = path.find_last_of('/');
    if (pos == 0) {
        return path.substr(pos, 1);
    }
    return path.substr(0, pos);
};

std::string_view Basename(std::string_view path) {
    auto pos = path.find_last_of('/');
    return path.substr(pos + 1);
}

std::string CollapseSlashes(std::string_view path) {
    if (path.empty()) {
        return static_cast<std::string>(path);
    }
    size_t pref_pos = 0;
    size_t suff_pos = path.size() - 1;

    while (pref_pos < suff_pos && path[pref_pos] == '/') {
        ++pref_pos;
    }
    while (suff_pos > pref_pos && path[suff_pos] == '/') {
        --suff_pos;
    }
    if (pref_pos == suff_pos) {
        if (pref_pos + 1 == path.size()) {
            return static_cast<std::string>(path.substr(0, 1));
        } else {
            return static_cast<std::string>(path.substr(pref_pos - 1, 3));
        }
    }

    bool need_allocation = false;
    for (size_t i = pref_pos + 1; i <= suff_pos; ++i) {
        if (path[i] == path[i - 1] && path[i] == '/') {
            need_allocation = true;
        }
    }

    if (!need_allocation) {
        if (path.back() == '/') {
            return static_cast<std::string>(path.substr(pref_pos - 1, suff_pos - pref_pos + 1));
        } else {
            return static_cast<std::string>(path.substr(pref_pos));
        }
    }

    std::string result(path);
    size_t it = 1;

    for (size_t i = 1; i < path.size(); ++i) {
        if (!(result[i] == result[i - 1] && result[i] == '/')) {
            result[it++] = result[i];
        }
    }

    result.resize(it);
    return result;
};

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter) {
    size_t result_size = delimiter.size() * strings.size();
    for (const auto& i : strings) {
        result_size += i.size();
    }

    std::string result(result_size, ' ');

    size_t result_position = 0;

    for (const auto& string : strings) {
        for (size_t i = 0; i < string.size(); ++i) {
            result[result_position++] = string[i];
        }
        for (size_t i = 0; i < delimiter.size(); ++i) {
            result[result_position++] = delimiter[i];
        }
    }

    if (!strings.empty()) {
        result.resize(result_size - delimiter.size());
    }

    return result;
};
