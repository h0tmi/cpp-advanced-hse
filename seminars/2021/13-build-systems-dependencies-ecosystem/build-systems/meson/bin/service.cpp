#include <lib/database.hpp>

#include <iostream>
#include <variant>

auto main() -> int {
    auto database = Database::Initialize();

    const auto result = database.Query("SELECT * FROM students;");

    if (std::holds_alternative<std::string>(result)) {
        std::cout << std::get<std::string>(result) << std::endl;
    }
}
