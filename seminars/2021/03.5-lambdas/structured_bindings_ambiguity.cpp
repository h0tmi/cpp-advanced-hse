#include <chrono>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;

struct TTNow {
    TimePoint earliest;
    TimePoint latest;
};

struct TrueTime {
    auto Now() -> TTNow {
        return {
            Clock::now() - Clock::duration{5},
            Clock::now() + Clock::duration{10},
        };
    }
};

auto main() -> int {
    auto tt = TrueTime{};
    auto tt_now = tt.Now();

    // Rust
    // let now = tt.Now();

    // Structured binding
    const auto [earliest, latest] = tt.Now();
}
