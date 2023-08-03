#include <gtest/gtest.h>

#include <hazard_ptr.h>

#include <vector>
#include <thread>

TEST(TestHazardPtr, SingleThread) {
    RegisterThread();

    std::atomic<int*> value = nullptr;
    value = new int{42};

    auto p = Acquire(&value);
    ASSERT_EQ(*p, 42);

    for (int i = 0; i < 100; i++) {
        Retire(value.exchange(new int{100500}));
    }
    Retire(value.exchange(nullptr));

    ASSERT_EQ(*p, 42);
    Release();

    UnregisterThread();
}

TEST(TestHazardPtr, ManyThreads) {
    std::atomic<int*> value = new int{42};

    std::vector<std::thread> threads;
    for (int j = 0; j < 10; j++) {
        threads.emplace_back([&] {
            RegisterThread();

            for (int i = 0; i < 100000; i++) {
                if (i % 123 == 0) {
                    Retire(value.exchange(new int{42}));
                } else {
                    auto p = Acquire(&value);
                    if (p) {
                        EXPECT_EQ(*p, 42);
                    }
                    Release();
                }
            }

            if (auto ptr = value.exchange(nullptr); ptr) {
                Retire(ptr);
            }

            UnregisterThread();
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}
