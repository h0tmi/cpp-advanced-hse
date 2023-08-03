#include <fmt/format.h>

#include <date/date.h>

#include <chrono>
#include <iostream>

template <>
struct fmt::formatter<date::year_month_day> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        const auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw format_error("invalid format");
        }
        return it;
    }

    template <typename FormatContext>
    auto format(const date::year_month_day& p, FormatContext& ctx) -> decltype(ctx.out()) {
        auto oss = std::ostringstream{};
        oss << p;
        return format_to(ctx.out(), "{}", oss.str());
    }
};

auto main(const int argc, const char* argv[]) -> int {
    const auto* name = argc <= 1 ? "World" : argv[1];
    std::cout << fmt::format("Hello, {}!\n", name);

    const auto today =
        date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())};
    std::cout << fmt::format("Auditing your visit at {}", today);
}
