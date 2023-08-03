#include "rw_lock.h"

#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>

#include <gtest/gtest.h>

std::chrono::high_resolution_clock::time_point Now() {
    return std::chrono::high_resolution_clock::now();
}

double ElapsedTime(std::chrono::high_resolution_clock::time_point start) {
    using std::chrono::duration;
    duration<double> time_span = std::chrono::duration_cast<duration<double>>(Now() - start);
    return time_span.count();
}

TEST(Correctness, Increment) {
    RWLock rw_lock;
    int threads_count = 4;
    double time_limit = 1.0;
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    int write_count = 0;
    std::atomic<int> read_count(0);
    int i_am_not_thread_safe = 0;

    for (int i = 0; i < threads_count; ++i) {
        if (i % 2 == 0) {
            threads.emplace_back([time_limit, &read_count, &rw_lock, &i_am_not_thread_safe]() {
                auto start = Now();
                while (ElapsedTime(start) < time_limit) {
                    rw_lock.Read([&read_count, &i_am_not_thread_safe]() {
                        ++read_count;
                        ASSERT_GE(i_am_not_thread_safe, 0);  // just to force reading
                    });
                }
            });
        } else {
            threads.emplace_back([time_limit, &write_count, &rw_lock, &i_am_not_thread_safe]() {
                auto start = Now();
                while (ElapsedTime(start) < time_limit) {
                    rw_lock.Write([&write_count, &i_am_not_thread_safe]() {
                        ++write_count;
                        ++i_am_not_thread_safe;  // writing to thread unsafe
                    });
                }
            });
        }
    }

    for (auto& cur : threads) {
        cur.join();
    }

    std::cerr << "read count " << read_count << ", write count " << write_count << "\n";

    ASSERT_GT(read_count, 10'000);
    ASSERT_GT(write_count, 10'000);
}

TEST(Concurrency, RLock) {
    double time_limit = 0.5;
    RWLock rw_lock;
    int max_threads_count = std::thread::hardware_concurrency();
    int threads_count = std::max(4, max_threads_count);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    auto start = Now();
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back([time_limit, &rw_lock]() {
            rw_lock.Read([time_limit]() {
                auto start = Now();
                while (ElapsedTime(start) < time_limit) {
                    // do nothing
                }
            });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto elapsed = ElapsedTime(start);
    ASSERT_LT(elapsed, time_limit * 2);
}

TEST(Concurrency, OnlyWritingOrReading) {
    double time_limit = 1;
    RWLock rw_lock;
    int max_threads_count = std::thread::hardware_concurrency();
    int threads_count = std::max(4, max_threads_count);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);

    std::atomic<bool> reading_flag(false);
    std::atomic<bool> writing_flag(false);

    for (int i = 0; i < threads_count; ++i) {
        if (i % 2 == 0) {
            threads.emplace_back([time_limit, &writing_flag, &reading_flag, &rw_lock]() {
                auto start = Now();
                while (ElapsedTime(start) < time_limit) {
                    rw_lock.Read([&writing_flag, &reading_flag]() {
                        reading_flag = true;
                        ASSERT_EQ(false, writing_flag.load());
                        reading_flag = false;
                    });
                }
            });
        } else {
            threads.emplace_back([time_limit, &reading_flag, &writing_flag, &rw_lock]() {
                auto start = Now();
                while (ElapsedTime(start) < time_limit) {
                    rw_lock.Write([&reading_flag, &writing_flag]() {
                        writing_flag = true;
                        ASSERT_EQ(false, reading_flag.load());
                        writing_flag = false;
                    });
                }
            });
        }
    }

    for (auto& cur : threads) {
        cur.join();
    }
}
