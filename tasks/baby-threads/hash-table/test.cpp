#include <concurrent_hash_map.h>
#include "commons.h"

#include <string>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>
#include <functional>
#include <stdexcept>
#include <queue>
#include <algorithm>
#include <thread>

#include <gtest/gtest.h>

using std::string;

const int kTestMaxIterations = 10000;
const int kSeed = 723834;

size_t PairHash(const std::pair<int, int>& x) {
    return 17239u * x.first + x.second;
}

TEST(Correctness, Constructors) {
    ConcurrentHashMap<int, int> first_table;
    ConcurrentHashMap<string, string> second_table(5, 16);
    ConcurrentHashMap<int, int64_t> third_table(17);

    ConcurrentHashMap<std::pair<int, int>, int, size_t (*)(const std::pair<int, int>&)> pair_table(
        PairHash);
    ASSERT_TRUE(pair_table.Insert(std::make_pair(1, 1), 2));
    ASSERT_EQ(pair_table.Size(), 1u);

    auto lambda = [](const std::pair<int, int>& x) { return PairHash(x); };
    ConcurrentHashMap<std::pair<int, int>, string, decltype(lambda)> new_pair_table(100, 1, lambda);
    ASSERT_TRUE(new_pair_table.Insert(std::make_pair(1, 2), "string"));
    ASSERT_EQ(new_pair_table.Size(), 1u);
}

TEST(Correctness, Operations) {
    ConcurrentHashMap<int, int> table;
    ASSERT_TRUE(table.Insert(3, 1));
    ASSERT_TRUE(table.Insert(2, 2));
    ASSERT_FALSE(table.Insert(2, 1));
    ASSERT_EQ(2, table.Find(2).second);
    ASSERT_FALSE(table.Find(5).first);
    ASSERT_EQ(2u, table.Size());
    table.Insert(5, 5);
    ASSERT_EQ(5, table.Find(5).second);

    ASSERT_TRUE(table.Erase(5));
    ASSERT_EQ(2u, table.Size());
    auto tmp = table.Find(2);
    ASSERT_TRUE(tmp.first);
    auto some_search = table.Find(5);
    ASSERT_FALSE(some_search.first);
    table.Clear();
    ASSERT_EQ(0u, table.Size());
    table.Insert(7, 3);
    ASSERT_EQ(std::make_pair(true, 3), table.Find(7));
}

TEST(Correctness, Constness) {
    ConcurrentHashMap<int, int> table;
    table.Insert(1, 1);
    table.Insert(2, 2);
    table.Insert(3, 3);
    const auto& ref = table;
    ASSERT_EQ(std::make_pair(true, 3), ref.Find(3));
    ASSERT_FALSE(ref.Find(4).first);
    ASSERT_EQ(1, ref.At(1));
    // NOLINTNEXTLINE
    ASSERT_THROW(ref.At(0), std::out_of_range);
}

TEST(Correctness, Clear) {
    ConcurrentHashMap<int, int> table;
    table.Insert(1, 1);
    table.Clear();
    ASSERT_TRUE(table.Insert(1, 1));
}

void CheckOutput(const ConcurrentHashMap<int, int>& table, std::vector<std::vector<int>> queries) {
    struct Item {
        int value;
        int thread_num;

        bool operator<(const Item& item) const {
            return value < item.value;
        }
    };
    std::priority_queue<Item> queue;
    int it = 0;
    for (auto& cur_queries : queries) {
        std::sort(cur_queries.begin(), cur_queries.end());
        queue.push(Item{cur_queries.back(), it++});
    }
    int unique_size = 0;
    int previous_value;
    bool is_first = true;
    while (!queue.empty()) {
        auto top = queue.top();
        queue.pop();
        if (is_first || top.value != previous_value) {
            ++unique_size;
        }
        is_first = false;
        previous_value = top.value;
        ASSERT_TRUE(table.Find(top.value).first);
        queries[top.thread_num].pop_back();
        if (!queries[top.thread_num].empty()) {
            queue.push(Item{queries[top.thread_num].back(), top.thread_num});
        }
    }
    ASSERT_EQ(unique_size, table.Size());
}

TEST(Concurrency, Insertions) {
    const int threads_count = 4;
    const int query_count = kTestMaxIterations;
    ConcurrentHashMap<int, int> table;
    std::vector<SimpleLogger> queries(threads_count);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back(MakeQueries<Random, SimpleLogger>, std::ref(table),
                             std::ref(queries[i]), query_count, QueryType::INSERT,
                             Random(kSeed + i));
    }
    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }

    CheckOutput(table, MoveToVectors(std::move(queries)));
}

TEST(Concurrency, Searching) {
    const int threads_count = 4;
    const int query_count = kTestMaxIterations;
    ConcurrentHashMap<int, int> table;
    std::vector<SimpleLogger> queries(threads_count / 2);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    DummyLogger dummy;
    threads.emplace_back(MakeQueries<Random, SimpleLogger>, std::ref(table), std::ref(queries[0]),
                         query_count, QueryType::INSERT, Random(kSeed - 1));
    threads.emplace_back(MakeQueries<Random, DummyLogger>, std::ref(table), std::ref(dummy),
                         query_count, QueryType::FIND, Random(kSeed - 2));
    threads.emplace_back(MakeQueries<Increment, SimpleLogger>, std::ref(table),
                         std::ref(queries[1]), query_count, QueryType::INSERT, Increment(-1));
    threads.emplace_back(MakeQueries<Increment, DummyLogger>, std::ref(table), std::ref(dummy),
                         query_count, QueryType::FIND, Increment(0));

    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }

    CheckOutput(table, MoveToVectors(std::move(queries)));
}

TEST(Concurrency, Erasing) {
    const int threads_count = 8;
    const int query_count = 1000;
    ConcurrentHashMap<int, int> table;
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    DummyLogger dummy;
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(MakeQueries<Random, DummyLogger>, std::ref(table), std::ref(dummy),
                             query_count, QueryType::INSERT, Random(kSeed + 100 * (i + 1)));
    }
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(MakeQueries<Random, DummyLogger>, std::ref(table), std::ref(dummy),
                             query_count, QueryType::ERASE, Random(kSeed + 100 * (i + 1)));
    }
    threads.emplace_back(MakeQueries<Increment, DummyLogger>, std::ref(table), std::ref(dummy),
                         query_count, QueryType::INSERT, Increment(100));
    threads.emplace_back(MakeQueries<Increment, DummyLogger>, std::ref(table), std::ref(dummy),
                         query_count, QueryType::ERASE, Increment(-100));
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(MakeQueries<Random, DummyLogger>, std::ref(table), std::ref(dummy),
                             query_count, QueryType::FIND, Random(19, 100, 200));
    }

    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }
}

TEST(Concurrency, Clear) {
    const int query_count = kTestMaxIterations / 2;
    DummyLogger dummy;
    ConcurrentHashMap<int, int> table;
    std::thread insert(MakeQueries<Random, DummyLogger>, std::ref(table), std::ref(dummy),
                       query_count, QueryType::INSERT, Random(kSeed + 17239));
    std::thread clear(MakeQueries<Increment, DummyLogger>, std::ref(table), std::ref(dummy),
                      query_count, QueryType::CLEAR, Increment(0));
    insert.join();
    clear.join();
}

template <class Func>
void MakeQueriesAndCheck(ConcurrentHashMap<int, int>& table, int query_count, int seed, Func func,
                         std::unordered_set<int>& result_set) {
    result_set.reserve(query_count);
    std::vector<int> inserted;
    inserted.reserve(query_count);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist;
    for (int i = 0; i < query_count; ++i) {
        double prob = dist(gen);
        if (prob < 0.6) {
            int value = func();
            inserted.push_back(value);
            table.Insert(value, 1);
            result_set.insert(value);
        } else if (prob < 0.8 && !inserted.empty()) {
            std::uniform_int_distribution<size_t> index(0, inserted.size() - 1);
            int value = inserted[index(gen)];
            table.Erase(value);
            result_set.erase(value);
        } else if (!inserted.empty()) {
            std::uniform_int_distribution<size_t> index(0, inserted.size() - 1);
            int value = inserted[index(gen)];
            ASSERT_EQ(result_set.count(value), table.Find(value).first);
        }
    }
}

TEST(Concurrency, OneThreadInteraction) {
    const int query_count = kTestMaxIterations;
    const int threads_count = 4;
    const int window = query_count * 100;

    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    ConcurrentHashMap<int, int> table;
    std::vector<std::unordered_set<int>> results(threads_count);
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back(
            MakeQueriesAndCheck<Random>, std::ref(table), query_count, kSeed + (i + 1) * 10,
            Random(kSeed - (i + 1) * 100, i * window + 1, (i + 1) * window), std::ref(results[i]));
    }

    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }

    size_t total_size = 0;
    for (const auto& set : results) {
        total_size += set.size();
        for (int x : set) {
            ASSERT_TRUE(table.Find(x).first);
        }
    }
    ASSERT_EQ(total_size, table.Size());
}
