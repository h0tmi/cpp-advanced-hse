#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> ans;

    for (const auto& i : items) {
        ans.push_back(std::make_unique<string>(i->data()));
    }
    return ans;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> ans;
    std::unordered_map<string, std::shared_ptr<string>> checker;

    for (const auto& i : items) {
        auto [it, _] = checker.try_emplace(i->data(), std::make_shared<string>(i->data()));
        ans.push_back(it->second);
    }

    return ans;
}
