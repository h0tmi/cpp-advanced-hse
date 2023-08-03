#include <gtest/gtest.h>
#include <mutex.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <sys/resource.h>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

TEST(Correctness, Simple) {
    Mutex spin;
    std::thread first([&spin]() {
        spin.Lock();
        std::this_thread::sleep_for(milliseconds(1500));
        spin.Unlock();
    });
    std::this_thread::sleep_for(milliseconds(300));
    std::thread second([&spin]() {
        auto start = high_resolution_clock::now();
        spin.Lock();
        auto end = high_resolution_clock::now();
        auto duration = (duration_cast<milliseconds>(end - start)).count();
        ASSERT_LT(1100, duration);
        ASSERT_LT(duration, 1300);
        spin.Unlock();
    });
    first.join();
    second.join();
}

TEST(Concurrency, Simple) {
    int threads_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    int counter = 0;
    Mutex mutex;
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back([&counter, &mutex]() {
            for (int j = 0; j < 100; ++j) {
                mutex.Lock();
                ++counter;
                mutex.Unlock();
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(100 * threads_count, counter);
}

namespace {
float GetUserCpuUsageSec() {
    rusage cpu_usage;
    if (::getrusage(RUSAGE_SELF, &cpu_usage) != 0) {
        throw std::runtime_error(std::string("Call to rusage failed ") + strerror(errno));
    }
    return cpu_usage.ru_utime.tv_sec + 1e-6 * cpu_usage.ru_utime.tv_usec;
}
}  // namespace

TEST(Concurrency, Spinlock) {
    Mutex mutex;
    std::atomic<bool> holder_is_ready{false};
    std::thread holder{[&holder_is_ready, &mutex]() {
        mutex.Lock();
        holder_is_ready = true;
        auto starting_cpu_usage = GetUserCpuUsageSec();
        std::this_thread::sleep_for(milliseconds(1000));
        auto current_cpu_usage = GetUserCpuUsageSec();
        mutex.Unlock();
        ASSERT_LT(current_cpu_usage - starting_cpu_usage, 0.1)
            << "your threads do not sleep, you probably implemented spinlock";
    }};

    while (!holder_is_ready) {
        // wait
    }

    std::thread waiter{[&mutex]() {
        mutex.Lock();  // thread should not consume cpu until holder finished
        mutex.Unlock();
    }};
    holder.join();
    waiter.join();
}
