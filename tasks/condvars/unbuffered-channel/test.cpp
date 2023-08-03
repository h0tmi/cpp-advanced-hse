#include <unbuffered_channel.h>
#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <chrono>
#include <algorithm>

std::chrono::high_resolution_clock::time_point Now() {
    return std::chrono::high_resolution_clock::now();
}

double ElapsedTime(std::chrono::high_resolution_clock::time_point start) {
    using std::chrono::duration;
    duration<double> time_span = std::chrono::duration_cast<duration<double>>(Now() - start);
    return time_span.count();
}

std::vector<int> Unpack(const std::vector<std::vector<int>>& values) {
    std::vector<int> all;
    for (const auto& vector : values) {
        all.insert(all.end(), vector.begin(), vector.end());
    }
    return all;
}

void CheckValues(const std::vector<std::vector<int>>& send_values,
                 const std::vector<std::vector<int>>& recv_values) {
    auto all_send = Unpack(send_values);
    auto all_recv = Unpack(recv_values);
    std::sort(all_send.begin(), all_send.end());
    std::sort(all_recv.begin(), all_recv.end());
    ASSERT_EQ(all_send, all_recv);
}

void RunTest(int senders_count, int receivers_count, int close_limit) {
    std::vector<std::thread> threads;
    threads.reserve(senders_count + receivers_count);
    UnbufferedChannel<int> channel;
    std::atomic<int> counter(0);
    std::vector<std::vector<int>> send_values(senders_count);
    std::atomic<bool> was_closed(false);
    for (int i = 0; i < senders_count; ++i) {
        threads.emplace_back([&channel, &counter, &send_values, &was_closed, i]() {
            for (;;) {
                int value = std::atomic_fetch_add(&counter, 1);
                try {
                    channel.Send(value);
                    send_values[i].push_back(value);
                } catch (std::runtime_error&) {
                    ASSERT_TRUE(was_closed.load());
                    break;
                }
            }
        });
    }
    std::vector<std::vector<int>> recv_values(receivers_count);
    for (int i = 0; i < receivers_count; ++i) {
        threads.emplace_back([&channel, &recv_values, &was_closed, i]() {
            for (;;) {
                auto value = channel.Recv();
                if (!value) {
                    ASSERT_TRUE(was_closed.load());
                    break;
                }
                recv_values[i].push_back(value.value());
            }
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(close_limit));
    was_closed = true;
    channel.Close();

    for (auto& cur : threads) {
        cur.join();
    }

    CheckValues(send_values, recv_values);
    if (senders_count == 1 && receivers_count == 1) {
        ASSERT_EQ(send_values[0], recv_values[0]);
    }
}

TEST(Correctness, Simple) {
    RunTest(1, 1, 200);
}

TEST(Correctness, Senders) {
    RunTest(4, 1, 200);
}

TEST(Correctness, Receivers) {
    RunTest(1, 6, 200);
}

TEST(Correctness, BigBuf) {
    RunTest(3, 3, 200);
}

enum class BlockType { kSender, kReceiver };

void BlockRun(BlockType block_type) {
    UnbufferedChannel<int> channel;
    int time_limit = 400;
    int iterations = 3;
    std::thread sender([&channel, time_limit, iterations, block_type]() {
        for (int i = 0; i < iterations; ++i) {
            if (block_type == BlockType::kReceiver) {
                std::this_thread::sleep_for(std::chrono::milliseconds(time_limit));
            }
            auto start = Now();
            channel.Send(i);
            int elapsed = static_cast<int>(ElapsedTime(start) * 1000);
            if (block_type == BlockType::kSender) {
                ASSERT_LT(time_limit - 100, elapsed);
                ASSERT_LT(elapsed, time_limit + 100);
            }
        }
        channel.Close();
    });
    std::thread receiver([&channel, time_limit, iterations, block_type]() {
        for (int i = 0; i < iterations; ++i) {
            if (block_type == BlockType::kSender) {
                std::this_thread::sleep_for(std::chrono::milliseconds(time_limit));
            }
            auto start = Now();
            auto value = channel.Recv();
            ASSERT_EQ(i, value.value());
            int elapsed = static_cast<int>(ElapsedTime(start) * 1000);
            if (block_type == BlockType::kReceiver) {
                ASSERT_LT(time_limit - 100, elapsed);
                ASSERT_LT(elapsed, time_limit + 100);
            }
        }
        ASSERT_FALSE(channel.Recv().has_value());
    });

    sender.join();
    receiver.join();
}

TEST(Block, Sender) {
    BlockRun(BlockType::kSender);
}

TEST(Block, Receiver) {
    BlockRun(BlockType::kReceiver);
}
