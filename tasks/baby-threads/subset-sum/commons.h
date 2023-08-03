#pragma once

#include <vector>
#include <util.h>

std::vector<int64_t> FillData(RandomGenerator& gen, size_t count, size_t part_size,
                              size_t garbage = 0) {
    const int64_t limit = 1e16;
    std::vector<int64_t> data;
    int64_t sum = 0;
    for (size_t i = 0; i < count - 1 - garbage; ++i) {
        auto elem = gen.GenInt(-limit, limit);
        data.push_back(elem);
        if (i >= part_size) {
            elem = -elem;
        }
        sum += elem;
    }
    data.push_back(sum);
    for (size_t i = 0; i < garbage; ++i) {
        data.push_back(gen.GenInt(-limit, limit));
    }
    gen.Shuffle(data.begin(), data.end());
    return data;
}
