#include <gtest/gtest.h>

#include <untyped_generator.h>

TEST(Untyped, Empty) {
    GeneratorUntyped gen([] {});
    ASSERT_TRUE(gen.get().empty());
}

TEST(Untyped, Simple) {
    GeneratorUntyped gen([] {
        yield(1);
        yield(3.);
    });
    ASSERT_EQ(boost::any_cast<int>(gen.get()), 1);
    ASSERT_EQ(boost::any_cast<double>(gen.get()), 3.);
    ASSERT_TRUE(gen.get().empty());
    ASSERT_TRUE(gen.get().empty());
}

TEST(Untyped, HelloWorld) {
    GeneratorUntyped gen([] {
        yield("hello");
        yield(" world");
        yield(1);
    });
    std::string result;
    while (true) {
        boost::any res = gen.get();
        if (res.empty()) {
            break;
        }
        if (auto str = boost::any_cast<const char*>(&res)) {
            result += *str;
        } else if (auto val = boost::any_cast<int>(&res)) {
            ASSERT_EQ(*val, 1);
        } else {
            ASSERT_TRUE(false);
        }
    }
    ASSERT_EQ(result, "hello world");
}
