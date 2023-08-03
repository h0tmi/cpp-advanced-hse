#include <gtest/gtest.h>

#include <thread>
#include <chrono>
#include <atomic>

#include <executors.h>

struct FutureTest : public ::testing::Test {
    std::shared_ptr<Executor> pool;

    FutureTest() {
        pool = MakeThreadPoolExecutor(2);
    }
};

TEST_F(FutureTest, InvokeVoid) {
    int x = 0;
    auto future = pool->Invoke<Unit>([&]() -> Unit {
        x = 42;

        return Unit{};
    });

    future->Get();
    ASSERT_EQ(x, 42);
}

TEST_F(FutureTest, InvokeString) {
    auto future = pool->Invoke<std::string>([]() { return "Hello World"; });

    ASSERT_EQ(future->Get(), std::string("Hello World"));
}

TEST_F(FutureTest, InvokeException) {
    auto future = pool->Invoke<Unit>([]() -> Unit { throw std::logic_error("Test"); });

    ASSERT_THROW(future->Get(), std::logic_error);
}

TEST_F(FutureTest, Then) {
    auto future_a = pool->Invoke<std::string>([]() { return std::string("Foo Bar"); });

    auto future_b = pool->Then<Unit>(future_a, [future_a]() {
        EXPECT_TRUE(future_a->IsFinished());
        EXPECT_EQ(future_a->Get(), std::string("Foo Bar"));

        return Unit{};
    });

    future_b->Get();
    EXPECT_TRUE(future_b->IsFinished());
}

TEST_F(FutureTest, ThenIsNonBlocking) {
    auto start = std::chrono::system_clock::now();

    auto future_a = pool->Invoke<std::string>([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return std::string("Foo Bar");
    });

    auto future_b = pool->Then<Unit>(future_a, [future_a]() { return Unit{}; });

    auto delta = std::chrono::system_clock::now() - start;
    EXPECT_LE(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count(), 50);
}

TEST_F(FutureTest, WhenAll) {
    const size_t n = 100;
    std::atomic<size_t> count{0};

    std::vector<FuturePtr<size_t>> all;
    for (size_t i = 0; i < n; i++) {
        all.emplace_back(pool->Invoke<size_t>([&, i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            count++;
            return i;
        }));
    }

    auto res_feature = pool->WhenAll(all);
    auto results = res_feature->Get();
    ASSERT_EQ(n, count.load());
    ASSERT_EQ(n, results.size());
    for (size_t i = 0; i < n; i++) {
        EXPECT_EQ(results[i], i);
    }
}

TEST_F(FutureTest, DISABLED_WhenFirst) {
    auto start = std::chrono::system_clock::now();
    auto first_future = pool->Invoke<int>([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 1;
    });

    auto last_future = pool->Invoke<int>([] { return 2; });

    auto res_feature = pool->WhenFirst(std::vector<FuturePtr<int>>{first_future, last_future});
    auto result = res_feature->Get();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - start);

    ASSERT_EQ(2, result);
    ASSERT_LE(time.count(), 50);
}

TEST_F(FutureTest, WhenAllBeforeDeadline) {
    const size_t n = 10;
    auto start = std::chrono::system_clock::now();

    std::vector<FuturePtr<int>> all;
    for (size_t i = 0; i < n; i++) {
        all.push_back(pool->Invoke<int>([&, i] { return i; }));
    }

    auto slow_future = pool->Invoke<Unit>([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return Unit{};
    });

    for (size_t i = 0; i < n; i++) {
        all.push_back(pool->Then<int>(slow_future, [&, i] { return n + i; }));
    }

    auto res_feature = pool->WhenAllBeforeDeadline(all, start + std::chrono::milliseconds(50));
    auto result = res_feature->Get();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - start);

    ASSERT_EQ(result.size(), n);
    ASSERT_LE(time.count(), 80);
}
