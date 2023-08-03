#pragma once

#include <string>
#include <optional>

void CheckImage(const std::string& filename, const std::string& expected_comment = "",
                std::optional<std::string> output_filename = std::nullopt);

void ExpectFail(const std::string& filename);
