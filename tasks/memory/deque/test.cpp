#include <catch.hpp>

#include <string>
#include <vector>
#include <random>
#include <deque>

#include <deque.h>

void Check(const Deque& actual, const std::vector<int>& expected) {
    REQUIRE(actual.Size() == expected.size());
    for (size_t i = 0; i < actual.Size(); ++i) {
        REQUIRE(actual[i] == expected[i]);
    }
}

TEST_CASE("Deque has constructors", "[deque]") {
    {
        Deque a;
        REQUIRE(a.Size() == 0u);
    }
    {
        Deque a{1, 2, 3, 4};
        Check(a, std::vector<int>{1, 2, 3, 4});
    }
    {
        Deque a(5);
        Check(a, std::vector<int>(5));
    }
}

TEST_CASE("Basic methods", "[deque]") {
    Deque a{1, 3, 5};
    Check(a, std::vector<int>{1, 3, 5});

    a.PopBack();
    Check(a, std::vector<int>{1, 3});
    a.PopFront();
    Check(a, std::vector<int>{3});
    a.PushFront(5);
    Check(a, std::vector<int>{5, 3});
    a.PushBack(1);
    Check(a, std::vector<int>{5, 3, 1});

    a.Clear();
    Check(a, std::vector<int>());

    a.PushBack(3);
    Deque b{2, 4};
    a.Swap(b);
    Check(a, std::vector<int>{2, 4});
    Check(b, std::vector<int>{3});
}

TEST_CASE("Modifications with []", "[deque]") {
    Deque a{9, 1, 1};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Check(a, std::vector<int>{1, 2, 3});
}

TEST_CASE("Memory layout", "[vector]") {
    Deque a(129);
    for (int i = 0; i < 127; ++i) {
        REQUIRE(&a[i] + 1 == &a[i + 1]);
    }
}

TEST_CASE("Reallocations", "[deque]") {
    Deque a;
    const int iterations = 1e6;
    std::vector<int*> addr;
    for (int i = 0; i < iterations; ++i) {
        a.PushBack(i);
        addr.push_back(&a[i]);
    }

    for (int i = 0; i < iterations; ++i) {
        REQUIRE(*addr[i] == i);
    }
}

TEST_CASE("Copy correctness", "[vector]") {
    Deque a;
    Deque b(a);
    b.PushBack(1);
    Check(a, std::vector<int>());
    Check(b, std::vector<int>{1});

    b = b;
    Check(b, std::vector<int>{1});
    a = b;
    Check(a, std::vector<int>{1});

    b = std::move(a);
    Check(b, std::vector<int>{1});
    Deque c(std::move(b));
    Check(c, std::vector<int>{1});

    Deque d{3, 4, 5};
    Deque e(d);
    Check(e, std::vector<int>{3, 4, 5});
    d.Swap(c);
    Check(e, std::vector<int>{3, 4, 5});
    Check(d, std::vector<int>{1});
    Check(c, std::vector<int>{3, 4, 5});
}

TEST_CASE("Stress", "[deque]") {
    const int iterations = 1e6;
    Deque a;
    std::deque<int> b;
    std::mt19937 gen(735675);
    std::uniform_int_distribution<int> dist(1, 5);

    for (int i = 0; i < iterations; ++i) {
        a.PushFront(i);
        b.push_front(i);
    }

    for (int i = 0; i < iterations; ++i) {
        int code = dist(gen);
        int value = gen();
        if (code == 1) {
            a.PushFront(value);
            b.push_front(value);
        } else if (code == 2) {
            a.PushBack(value);
            b.push_back(value);
        } else if (code == 3) {
            a.PopFront();
            b.pop_front();
        } else if (code == 4) {
            a.PopBack();
            b.pop_back();
        } else {
            int index = static_cast<int>(value % a.Size());
            REQUIRE(a[index] == b[index]);
        }
    }
}

TEST_CASE("Empty correctness") {
    // There are some ways to make deque empty
    // We should test them all
    // In some ways we can cause memory leak
    const size_t test_size = 1e3;
    {
        // PushBack-PopBack case
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushBack(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopBack();
        }
        REQUIRE(a.Size() == 0u);
        Check(a, std::vector<int>());
    }

    {
        // PushBack-PopFront case
        // this case is broken in my implementation
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushBack(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopFront();
        }
        REQUIRE(a.Size() == 0u);
        Check(a, std::vector<int>());
    }

    {
        // PushFront-PopBack case
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushFront(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopBack();
        }
        REQUIRE(a.Size() == 0u);
        Check(a, std::vector<int>());
    }

    {
        // PushFront-PopFront case
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushFront(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopFront();
        }
        REQUIRE(a.Size() == 0u);
        Check(a, std::vector<int>());
    }
}

TEST_CASE("Correct work of cycled buffer") {
    Deque a;
    a.PushFront(1);
    a.PushBack(2);
    Check(a, std::vector<int>{1, 2});

    Deque b;
    b.PushBack(1);
    b.PushFront(2);
    Check(b, std::vector<int>{2, 1});

    // Check cycled buffer work after reallocation
    const int iterations = 128;

    std::vector<int> v{1, 2};
    for (int i = 0; i < iterations; ++i) {
        a.PushBack(i);
        v.push_back(i);
    }
    Check(a, v);

    std::deque<int> w{2, 1};
    for (int i = 0; i < iterations; ++i) {
        b.PushFront(i);
        w.push_front(i);
    }
    Check(b, std::vector<int>(w.begin(), w.end()));
}
