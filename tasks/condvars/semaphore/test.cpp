#include "sema.h"

#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

#include <gtest/gtest.h>

void RunTest(int threads_count, int concurrency_level) {
    Semaphore semaphore(concurrency_level);
    for (int i = 0; i < concurrency_level; ++i) {
        semaphore.Enter();
    }

    int time = 0;
    std::atomic<int> expected_value = 0;
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back([&time, &semaphore, i, &expected_value]() {
            semaphore.Enter([&time, i, &expected_value](int& value) {
                auto cur_time = time++;
                ASSERT_EQ(i, cur_time);
                ASSERT_EQ(expected_value.load(), value);
                --value;
                expected_value.fetch_sub(1);
            });
            semaphore.Leave();
            expected_value.fetch_add(1);
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
    }

    for (int i = 0; i < concurrency_level; ++i) {
        expected_value.fetch_add(1);
        semaphore.Leave();
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
    }

    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }
}

TEST(Order, NewTest) {
    int callback_invocations_count = 0;

    for (int i = 0; i < 10; ++i) {
        Semaphore semaphore(1);
        int time = 0;
        std::vector<std::thread> threads;
        threads.reserve(3);
        std::atomic<bool> flag = false;

        threads.emplace_back([&time, &semaphore, &flag, &callback_invocations_count]() {
            semaphore.Enter([&time, &callback_invocations_count](int& value) {
                ++callback_invocations_count;
                auto cur_time = time++;
                ASSERT_EQ(0, cur_time);
                --value;
            });
            while (!flag) {
            }
            semaphore.Leave();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        threads.emplace_back([&time, &semaphore, &callback_invocations_count]() {
            semaphore.Enter([&time, &callback_invocations_count](int& value) {
                ++callback_invocations_count;
                auto cur_time = time++;
                ASSERT_EQ(1, cur_time);
                --value;
            });
            semaphore.Leave();
        });

        threads.emplace_back([&time, &semaphore, &flag, &callback_invocations_count]() {
            while (!flag) {
            }
            semaphore.Enter([&time, &callback_invocations_count](int& value) {
                ++callback_invocations_count;
                auto cur_time = time++;
                ASSERT_EQ(2, cur_time);
                --value;
            });
            semaphore.Leave();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        flag = true;

        for (auto& cur : threads) {
            cur.join();
        }
    }

    ASSERT_EQ(callback_invocations_count, 30);
}

TEST(Order, Mutex) {
    RunTest(4, 1);
}

TEST(Order, Semaphore) {
    RunTest(4, 3);
}
