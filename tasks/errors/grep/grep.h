#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "utf8.h"
#include "utf8/checked.h"
#include "utf8/core.h"
#include "utf8/unchecked.h"

using std::optional;

struct GrepOptions {
    optional<size_t> look_ahead_length;
    size_t max_matches_per_line;

    GrepOptions() {
        max_matches_per_line = 10;
    }

    GrepOptions(size_t look_ahead_length) : GrepOptions() {
        this->look_ahead_length = look_ahead_length;
    }

    GrepOptions(optional<size_t> look_ahead_length, size_t max_matches_per_line) {
        this->look_ahead_length = look_ahead_length;
        this->max_matches_per_line = max_matches_per_line;
    }
};

template <class Visitor>
void GrepInFile(const std::filesystem::directory_entry& file, const std::string& base_pattern,
                Visitor visitor, const GrepOptions& options) {
    std::ifstream file_stream(file.path().string());

    if (!file_stream.is_open()) {
        throw;
    }

    std::vector<int16_t> pattern;
    utf8::utf8to16(base_pattern.begin(), base_pattern.end(), std::back_inserter(pattern));

    std::string current_line;
    for (size_t line_number = 1; std::getline(file_stream, current_line); ++line_number) {
        std::vector<short> current_string;
        current_string.reserve(current_line.size());
        try {
            utf8::utf8to16(current_line.begin(), current_line.end(),
                           std::back_inserter(current_string));
        } catch (...) {
            throw;
        }
        size_t column_number;
        auto current_match_it =
            std::search(current_string.begin(), current_string.end(),
                        std::boyer_moore_searcher(pattern.begin(), pattern.end()));
        auto saved_match = current_match_it;
        size_t max_matches_cnt = 0;

        while (max_matches_cnt < options.max_matches_per_line &&
               current_match_it != current_string.end()) {

            try {
                column_number = utf8::distance(current_string.begin(), current_match_it) + 1;
            } catch (...) {
                column_number =
                    utf8::unchecked::distance(current_string.begin(), current_match_it) + 1;
            }

            if (options.look_ahead_length.has_value()) {
                utf8::unchecked::advance(current_match_it, pattern.size());
                auto current_match_it_end = current_match_it;

                if (column_number + pattern.size() + options.look_ahead_length.value() - 1 <
                    current_string.size()) {
                    utf8::unchecked::advance(current_match_it_end,
                                             options.look_ahead_length.value());
                } else {
                    current_match_it_end = current_string.end();
                }
                std::string context;
                utf8::unchecked::utf16to8(current_match_it, current_match_it_end,
                                          std::back_inserter(context));
                visitor.OnMatch(file.path().string(), line_number,
                                saved_match - current_string.begin() + 1, context);
            } else {
                visitor.OnMatch(file.path().string(), line_number, column_number, std::nullopt);
            }
            current_match_it = saved_match;
            ++max_matches_cnt;
            current_match_it =
                std::search(++current_match_it, current_string.end(),
                            std::boyer_moore_searcher(pattern.begin(), pattern.end()));
            saved_match = current_match_it;
        }
    }
}

template <class Visitor>
void Grep(const std::string& path, const std::string& pattern, Visitor visitor,
          const GrepOptions& options) {
    try {
        if (std::filesystem::is_regular_file(path)) {
            GrepInFile(std::filesystem::directory_entry(path), pattern, visitor, options);
        } else {
            for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
                if (file.is_regular_file()) {
                    GrepInFile(file, pattern, visitor, options);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        // throw e;
    }
}
