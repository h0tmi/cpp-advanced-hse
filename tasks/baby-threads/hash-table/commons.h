#pragma once

#include <concurrent_hash_map.h>

#include <random>
#include <thread>
#include <vector>
#include <limits>

enum class QueryType { INSERT, ERASE, FIND, CLEAR };

class Increment {
public:
    explicit Increment(int start) : start_(start) {
    }
    int operator()() {
        return start_++;
    }

private:
    int start_;
};

class EqualLowBits {
public:
    explicit EqualLowBits(int low_bits_count) {
        start_ = 0;
        low_bits_count_ = low_bits_count;
        low_bits_ = 0;
    }
    int operator()() {
        int result = ((start_++) << low_bits_count_) + low_bits_;
        if (start_ == (1 << (sizeof(int) * 8 - low_bits_count_))) {
            start_ = 0;
            ++low_bits_;
        }
        return result;
    }

private:
    int start_, low_bits_count_;
    int low_bits_;
};

class Random {
public:
    explicit Random(int seed, int min = std::numeric_limits<int>::min(),
                    int max = std::numeric_limits<int>::max())
        : gen_(seed), dist_(min, max) {
    }
    int operator()() {
        return dist_(gen_);
    }

private:
    std::mt19937 gen_;
    std::uniform_int_distribution<int> dist_;
};

class SimpleLogger {
public:
    void StartLogging(int query_count) {
        log_.reserve(query_count);
    }

    void Log(int value) {
        log_.push_back(value);
    }

    std::vector<int>&& GetLog() {
        return std::move(log_);
    }

private:
    std::vector<int> log_;
};

class DummyLogger {
public:
    void StartLogging(int) {
    }
    void Log(int) {
    }
};

std::vector<std::vector<int>> MoveToVectors(std::vector<SimpleLogger>&& logs) {
    std::vector<std::vector<int>> result;
    result.reserve(logs.size());
    for (auto&& log : logs) {
        result.emplace_back(log.GetLog());
    }
    return result;
}

template <class Func, class Logger>
void MakeQueries(ConcurrentHashMap<int, int>& table, Logger& logger, int query_count,
                 QueryType query_type, Func func) {
    logger.StartLogging(query_count);
    for (int i = 0; i < query_count; ++i) {
        int value = func();
        logger.Log(value);
        switch (query_type) {
            case QueryType::INSERT:
                table.Insert(value, 1);
                break;
            case QueryType::ERASE:
                table.Erase(value);
                break;
            case QueryType::FIND:
                table.Find(value);
                break;
            case QueryType::CLEAR:
                table.Clear();
                break;
        }
    }
}
