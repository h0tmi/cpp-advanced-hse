#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> result;

    for (const auto& i : items) {
        result.emplace_back(std::make_unique<string>(i->data()));
    }

    return result;
}

std::vector<std::shared_ptr<string>> DeDuplicate(const std::vector<std::unique_ptr<string>>& items) {
    std::unordered_map<string, std::shared_ptr<string>> checker;
    std::vector<std::shared_ptr<string>> result;

    for (const auto& i : items) {
        const auto [it, _] = checker.try_emplace(i->data(), std::make_shared<string>(i->data()));

        result.emplace_back(it->second);
    }

    return result;
}
