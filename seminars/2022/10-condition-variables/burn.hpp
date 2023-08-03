#pragma once

#include <chrono>
#include <locale>


inline void BurnCpuUntil(std::chrono::system_clock::time_point deadline) {
    while (std::chrono::system_clock::now() < deadline) {
        // pass
    }
}

inline void BurnCpuFor(std::chrono::system_clock::duration delta) {
    BurnCpuUntil(std::chrono::system_clock::now() + delta);
}
