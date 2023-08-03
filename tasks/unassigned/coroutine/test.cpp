#include <gtest/gtest.h>
#include <thread>

#include <coroutine.h>

TEST(Simple, Start) {
    int counter = 0;
    Coroutine c([&] {
        ASSERT_EQ(counter, 0);
        ++counter;
    });
    ASSERT_EQ(counter, 1);
}

TEST(Simple, ResumeSuspend) {
    int counter = 0;
    Coroutine c([&] {
        ASSERT_EQ(counter, 0);
        ++counter;
        suspend();
        ASSERT_EQ(counter, 2);
        ++counter;
    });
    ASSERT_EQ(counter, 1);
    ++counter;
    c.resume();
    ASSERT_EQ(counter, 3);
}

TEST(Error, Suspend) {
    EXPECT_THROW(suspend(), std::runtime_error);
}

TEST(Error, Start) {
    int counter = 0;
    Coroutine c([&] {
        ASSERT_EQ(counter, 0);
        ++counter;
    });
    ASSERT_EQ(counter, 1);
    EXPECT_THROW(c.resume(), std::runtime_error);
    EXPECT_THROW(c.resume(), std::runtime_error);
}

TEST(Error, ResumeSuspend) {
    int counter = 0;
    Coroutine c([&] {
        EXPECT_THROW(c.resume(), std::runtime_error);
        ASSERT_EQ(counter, 0);
        ++counter;
        suspend();
        EXPECT_THROW(c.resume(), std::runtime_error);
        ASSERT_EQ(counter, 2);
        ++counter;
        EXPECT_THROW(c.resume(), std::runtime_error);
    });
    ASSERT_EQ(counter, 1);
    ++counter;
    c.resume();
    ASSERT_EQ(counter, 3);
    EXPECT_THROW(c.resume(), std::runtime_error);
}

void recursive(int invocation) {
    if (invocation) {
        recursive(invocation - 1);
    }
    suspend();
}

TEST(Loop, ResumeSuspendRecursive) {
    Coroutine c([] { recursive(30); });
    for (int i = 0; i <= 30; ++i) {
        c.resume();
    }
    EXPECT_THROW(c.resume(), std::runtime_error);
}

TEST(Ownership, Ptr) {
    std::vector<std::string> strs = {"hello", "world"};
    Coroutine c([strs = std::move(strs)]() mutable {
        suspend();
        ASSERT_STREQ(strs[0].c_str(), "hello");
        strs.push_back("!");
        suspend();
        ASSERT_STREQ(strs[1].c_str(), "world");
        suspend();
        ASSERT_STREQ(strs[2].c_str(), "!");
    });
    c.resume();
    c.resume();
    c.resume();
    EXPECT_THROW(c.resume(), std::runtime_error);
}

TEST(Threads, Recursive) {
    auto mt = [] {
        Coroutine c([] { recursive(100); });
        for (int i = 0; i <= 100; ++i) {
            c.resume();
        }
        EXPECT_THROW(c.resume(), std::runtime_error);
    };
    std::thread t(mt);
    std::thread t2(mt);
    mt();
    t.join();
    t2.join();
}

TEST(Pair, Simple) {
    int counter = 0;
    Coroutine c([&] {
        ASSERT_EQ(counter, 0);
        ++counter;
        suspend();
        ASSERT_EQ(counter, 4);
        ++counter;
    });
    ASSERT_EQ(counter, 1);
    ++counter;
    Coroutine c2([&] {
        ASSERT_EQ(counter, 2);
        ++counter;
        suspend();
        ASSERT_EQ(counter, 6);
        ++counter;
    });
    ASSERT_EQ(counter, 3);
    ++counter;
    c.resume();
    ASSERT_EQ(counter, 5);
    ++counter;
    EXPECT_THROW(c.resume(), std::runtime_error);
    c2.resume();
    ASSERT_EQ(counter, 7);
    EXPECT_THROW(c2.resume(), std::runtime_error);
}
