#pragma once

#include <chrono>
#include <climits>
#include <ostream>
#include <cstdlib>

namespace timer {

using WallClock = std::chrono::high_resolution_clock;

template <typename Duration>
double ToSeconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(d).count();
}

template <typename Duration>
Duration FromSeconds(double seconds) {
    return std::chrono::duration_cast<Duration>(std::chrono::duration<double>{seconds});
}

////////////////////////////////////////////////////////////////////////////////

struct Duration {
    WallClock::duration WallTime = {};
    WallClock::duration CpuTime = {};
};

inline std::ostream& operator<<(std::ostream& os, const Duration& d) {
    return os << "{cpu_time: " << ToSeconds(d.CpuTime) << "s, wall_time: " << ToSeconds(d.WallTime)
              << "s, ratio: " << ToSeconds(d.CpuTime) / ToSeconds(d.WallTime) << "x}";
}

////////////////////////////////////////////////////////////////////////////////

class Timer {
    struct Instant {
        WallClock::time_point WallTimePoint = {};
        clock_t CpuTimePoint = {};

        static Instant Now() {
            return Instant{
                .WallTimePoint = WallClock::now(),
                .CpuTimePoint = clock(),
            };
        }

        Duration operator-(const Instant& rhs) const {
            double cpu_seconds =
                static_cast<double>(CpuTimePoint - rhs.CpuTimePoint) / CLOCKS_PER_SEC;
            return {
                .WallTime = WallTimePoint - rhs.WallTimePoint,
                .CpuTime = FromSeconds<WallClock::duration>(cpu_seconds),
            };
        }
    };

public:
    Timer() : start_{Instant::Now()} {
    }

    Duration Elapsed() const {
        return Instant::Now() - start_;
    }

private:
    Instant start_;
};

}  // namespace timer
