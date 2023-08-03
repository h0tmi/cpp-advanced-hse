#pragma once

#include <sstream>
#include <random>
#include <array>

class Fuzzer {
public:
    Fuzzer() : gen_(kSeed) {
    }

    std::string Next() {
        std::uniform_int_distribution<uint32_t> dist(1, sequences.size());
        ss_.str("");
        std::shuffle(sequences.begin(), sequences.end(), gen_);
        uint32_t len = dist(gen_);

        for (uint32_t j = 0; j < len; ++j) {
            ss_ << sequences[j] << " ";
        }

        return ss_.str();
    }

private:
    static inline std::array sequences = {"(", "(",  "(", "(",  ")", ")",      ")",
                                          ")", "-",  "+", "#t", "",  "symbol", "0",
                                          "1", "-2", ".", ".",  ".", ".",      "'"};

    static inline constexpr uint32_t kSeed = 16;

    std::stringstream ss_;
    std::mt19937 gen_;
};
