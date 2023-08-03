#include <gtest/gtest.h>

#include "sync_map.h"

class TestSyncMap : public ::testing::Test {
public:
    void SetUp() override {
        RegisterThread();
    }

    void TearDown() override {
        UnregisterThread();
    }
};

TEST_F(TestSyncMap, Simple) {
    SyncMap<int, int> map;

    int value;
    EXPECT_FALSE(map.Lookup(0, &value));

    EXPECT_TRUE(map.Insert(0, 42));
    EXPECT_TRUE(map.Lookup(0, &value));
    EXPECT_EQ(value, 42);

    EXPECT_FALSE(map.Insert(0, 42));
}

TEST_F(TestSyncMap, UpgradeToReadOnly) {
    SyncMap<int, int> map;

    map.Insert(0, 42);
    map.Insert(1, 42);
    map.Insert(2, 42);

    for (int i = 0; i < 1024; i++) {
        int value;
        EXPECT_TRUE(map.Lookup(0, &value));
        EXPECT_EQ(value, 42);
    }
}
