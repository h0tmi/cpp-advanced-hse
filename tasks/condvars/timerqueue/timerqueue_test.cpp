#include "timerqueue.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <atomic>

using namespace std::chrono_literals;

namespace {

auto Now() {
    return std::chrono::system_clock::now();
}

}  // namespace

TEST(TimerQueue, AddGet) {
    auto now = Now();

    TimerQueue<int> queue;
    queue.Add(0, now + 1ms);
    queue.Add(1, now + 10ms);
    queue.Add(2, now + 5ms);

    ASSERT_EQ(0, queue.Pop());
    ASSERT_EQ(2, queue.Pop());
    ASSERT_EQ(1, queue.Pop());
}

TEST(TimerQueue, Conflict) {
    auto now = Now();

    TimerQueue<int> queue;
    queue.Add(0, now);
    queue.Add(1, now);
    queue.Add(2, now);

    std::set<int> expected{0, 1, 2};
    std::set<int> results{queue.Pop(), queue.Pop(), queue.Pop()};
    ASSERT_EQ(expected, results);
}

TEST(TimerQueue, Blocking) {
    auto now = Now();

    TimerQueue<int> queue;
    queue.Add(0, now + 500ms);
    queue.Pop();

    ASSERT_TRUE(std::chrono::system_clock::now() >= now + 500ms);
}

TEST(TimerQueue, TwoThreads) {
    auto now = Now();

    TimerQueue<int> queue;

    std::atomic<bool> finished = false;
    std::thread worker([&] {
        queue.Pop();
        finished = true;
    });

    std::this_thread::sleep_for(500ms);
    EXPECT_FALSE(finished);

    queue.Add(0, now);
    worker.join();
}

TEST(TimerQueue, WakeUp) {
    TimerQueue<std::chrono::system_clock::time_point> queue;

    std::thread worker([&] {
        for (int i = 0; i < 5; ++i) {
            auto res = queue.Pop();
            auto now = Now();
            EXPECT_LE(res, now);
            EXPECT_GT(res + 50ms, now);
        }
    });

    for (int i = 0; i < 5; ++i) {
        auto now = Now();
        queue.Add(now, now);
        std::this_thread::sleep_for(200ms);
    }

    worker.join();
}

TEST(TimerQueue, TimerReschedule) {
    auto now = std::chrono::system_clock::now();

    TimerQueue<int> queue;
    queue.Add(0, now + 10s);

    std::thread worker([&] { queue.Pop(); });

    std::this_thread::sleep_for(500ms);

    queue.Add(1, now);
    worker.join();

    ASSERT_TRUE(std::chrono::system_clock::now() < now + 1s);
}
