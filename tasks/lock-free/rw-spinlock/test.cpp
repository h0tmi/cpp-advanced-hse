#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <atomic>

#include <rw_spinlock.h>

using std::literals::chrono_literals::operator""ms;

TEST(Correctness, Read) {
    std::atomic<int> counter = {0};

    RWSpinLock lock;
    lock.LockRead();
    std::thread t1([&] {
        lock.LockRead();
        counter++;
    });
    t1.join();
    ASSERT_EQ(counter.load(), 1);
}

TEST(Correctness, Write) {
    std::atomic<int> counter = {0};

    RWSpinLock lock;
    lock.LockWrite();
    std::thread t1([&] {
        lock.LockWrite();
        counter++;
    });
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(counter.load(), 0);
    lock.UnlockWrite();
    t1.join();
    ASSERT_EQ(counter.load(), 1);
}

TEST(Correctness, ReadWrite) {
    std::atomic<int> counter = {0};

    RWSpinLock lock;
    lock.LockRead();
    std::thread t1([&] {
        lock.LockWrite();
        counter++;
    });
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(counter.load(), 0);
    lock.UnlockRead();
    t1.join();
    ASSERT_EQ(counter.load(), 1);
}

TEST(Correctness, WriteRead) {
    std::atomic<int> counter = {0};

    RWSpinLock lock;
    lock.LockWrite();
    std::thread t1([&] {
        lock.LockRead();
        counter++;
    });
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(counter.load(), 0);
    lock.UnlockWrite();
    t1.join();
    ASSERT_EQ(counter.load(), 1);
}
