#pragma once

#include <string>
#include <variant>

class Database {
public:
    static auto Initialize() -> Database;

    struct Error {
        std::string message;
        int code;
    };

    using Result = std::variant<std::string, Error>;

    auto Query(const std::string& query) -> Result;
};
