#pragma once

namespace chromo {

struct Time {
    int since_unix_epoch;
};

auto Now() -> Time;

}  // namespace chromo
