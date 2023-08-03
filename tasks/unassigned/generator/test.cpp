#include <gtest/gtest.h>

#include <generator.h>

TEST(Generator, Empty) {
    Generator<int> gen([](auto&) {});
    ASSERT_EQ(gen.get(), nullptr);
}

TEST(Generator, Simple) {
    Generator<int> gen([](Yield<int>& yield) { yield.yield(1); });
    auto res = gen.get();
    ASSERT_NE(res, nullptr);
    ASSERT_EQ(*res, 1);
    ASSERT_EQ(gen.get(), nullptr);
}

TEST(Generator, String) {
    Generator<std::string> gen([](auto& yield) {
        yield.yield("hello");
        yield.yield(" world");
        yield.yield("!");
    });
    std::string str;
    while (auto res = gen.get()) {
        str += *res;
    }
    ASSERT_STREQ(str.c_str(), "hello world!");
}

auto makeRange(int to) {
    return Generator<int>([to](auto& yield) {
        for (int i = 0; i < to; ++i) {
            yield.yield(i);
        }
    });
}

TEST(Generator, MakeRange) {
    auto range = makeRange(10);
    int sum = 0;
    while (auto res = range.get()) {
        sum += *res;
    }
    ASSERT_EQ(sum, 45);
}
