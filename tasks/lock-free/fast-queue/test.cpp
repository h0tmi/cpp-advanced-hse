#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

#include <mpmc.h>

TEST(Correctness, Enqueue) {
    MPMCBoundedQueue<int> queue(2);
    ASSERT_TRUE(queue.Enqueue(2));
    ASSERT_TRUE(queue.Enqueue(2));
    ASSERT_FALSE(queue.Enqueue(2));
    ASSERT_FALSE(queue.Enqueue(2));
}

TEST(Correctness, Dequeue) {
    int val;
    MPMCBoundedQueue<int> queue(2);
    ASSERT_FALSE(queue.Dequeue(val));
    ASSERT_FALSE(queue.Dequeue(val));
}

TEST(Correctness, EnqueueDequeue) {
    int val = 0;
    MPMCBoundedQueue<int> queue(2);
    ASSERT_TRUE(queue.Enqueue(1));
    ASSERT_TRUE(queue.Dequeue(val));
    ASSERT_EQ(val, 1);
    ASSERT_FALSE(queue.Dequeue(val));

    ASSERT_TRUE(queue.Enqueue(2));
    ASSERT_TRUE(queue.Enqueue(3));
    ASSERT_FALSE(queue.Enqueue(4));

    ASSERT_TRUE(queue.Dequeue(val));
    ASSERT_EQ(val, 2);
    ASSERT_TRUE(queue.Dequeue(val));
    ASSERT_EQ(val, 3);

    ASSERT_FALSE(queue.Dequeue(val));
}

TEST(Correctness, NoSpuriousFails) {
    const int n = 1024 * 1024;
    const int n_threads = 4;
    MPMCBoundedQueue<int> queue(n * n_threads);

    std::vector<std::thread> writers;
    for (int i = 0; i < n_threads; i++) {
        writers.emplace_back([&] {
            for (int j = 0; j < n; ++j) {
                ASSERT_TRUE(queue.Enqueue(0));
            }
        });
    }

    for (auto& t : writers) {
        t.join();
    }

    std::vector<std::thread> readers;
    for (int i = 0; i < n_threads; i++) {
        readers.emplace_back([&] {
            for (int j = 0; j < n; ++j) {
                int k;
                ASSERT_TRUE(queue.Dequeue(k));
            }
        });
    }

    for (auto& t : readers) {
        t.join();
    }
}

TEST(Correctness, NoQueueLock) {
    const int n = 1024 * 1024;
    const int n_threads = 8;
    MPMCBoundedQueue<int> queue(64);

    std::vector<std::thread> threads;
    std::atomic<int> ids = {0};
    for (int i = 0; i < n_threads; i++) {
        threads.emplace_back([&] {
            int id = ids++;
            if (id % 2) {
                for (int j = 0; j < n; ++j) {
                    queue.Enqueue(0);
                }
            } else {
                for (int j = 0; j < n; ++j) {
                    int k;
                    queue.Dequeue(k);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int k;
    while (queue.Dequeue(k)) {
        queue.Dequeue(k);
    }
    ASSERT_TRUE(queue.Enqueue(0));
    ASSERT_TRUE(queue.Dequeue(k));
    ASSERT_EQ(k, 0);
}
