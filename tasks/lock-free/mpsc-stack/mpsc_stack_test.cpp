#include <gtest/gtest.h>

#include <mpsc_stack.h>

#include <algorithm>
#include <unordered_set>
#include <thread>

TEST(TestMPSCStack, PushPop) {
    MPSCStack<int> stack;

    stack.Push(1);
    stack.Push(2);

    auto value = stack.Pop();
    EXPECT_TRUE(value);
    EXPECT_EQ(2, *value);

    value = stack.Pop();
    EXPECT_TRUE(value);
    EXPECT_EQ(1, *value);

    value = stack.Pop();
    EXPECT_FALSE(value);
}

TEST(TestMPSCStack, SingleThread) {
    MPSCStack<int> stack;

    std::vector<int> result;
    for (int i = 0; i < 1024; i++) {
        result.push_back(i);
        stack.Push(i);
    }

    std::vector<int> dequeued;
    stack.DequeueAll([&](int j) { dequeued.push_back(j); });

    std::reverse(dequeued.begin(), dequeued.end());
    EXPECT_EQ(dequeued, result);
}

TEST(TestMPSCStack, Destructor) {
    MPSCStack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
}

TEST(TestMPSCStack, Threaded) {
    std::unordered_set<int> all_dequeued;
    MPSCStack<int> stack;

    std::atomic<bool> stop = false;
    std::thread dequeuer([&] {
        auto save = [&](int value) {
            auto [it, ok] = all_dequeued.emplace(value);
            EXPECT_TRUE(ok);
        };

        while (!stop) {
            stack.DequeueAll(save);
        }

        stack.DequeueAll(save);
    });

    constexpr int kN = 100000;
    for (int i = 0; i < kN; i++) {
        stack.Push(i);
    }

    stop = true;
    dequeuer.join();
    EXPECT_EQ(kN, all_dequeued.size());
}
