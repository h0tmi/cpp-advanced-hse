#include <catch.hpp>

#include <string>
#include <vector>
#include <random>
#include <memory>

#include <list.h>

TEST_CASE("Empty list works", "[List]") {
    List<int> l1;
    List<std::string> l2;
    List<std::vector<int>> l3;

    REQUIRE(l1.IsEmpty());
    REQUIRE(l2.IsEmpty());
    REQUIRE(l3.IsEmpty());

    REQUIRE(l1.Size() == 0);
    REQUIRE(l2.Size() == 0);
    REQUIRE(l3.Size() == 0);
}

TEST_CASE("Back() and PushBack() methods work", "[List]") {
    List<int> l;
    l.PushBack(0);
    REQUIRE(l.Back() == 0);
    l.PushBack(1);
    REQUIRE(l.Back() == 1);
    REQUIRE(l.Size() == 2);
    REQUIRE(!l.IsEmpty());
    l.PopBack();
    REQUIRE(!l.IsEmpty());
    REQUIRE(l.Size() == 1);
    REQUIRE(l.Back() == 0);
    l.PopBack();
    REQUIRE(l.IsEmpty());
    REQUIRE(l.Size() == 0);
}

TEST_CASE("Front() and PushFront() methods work", "[List]") {
    List<int> l;
    l.PushFront(0);
    REQUIRE(l.Front() == 0);
    REQUIRE(l.Back() == 0);
    l.PushFront(1);
    REQUIRE(l.Back() == 0);
    REQUIRE(l.Front() == 1);
    REQUIRE(l.Size() == 2);
    REQUIRE(!l.IsEmpty());
    l.PopFront();
    REQUIRE(!l.IsEmpty());
    REQUIRE(l.Size() == 1);
    REQUIRE(l.Front() == 0);
    l.PopBack();
    REQUIRE(l.IsEmpty());
    REQUIRE(l.Size() == 0);
}

int move_deleted = 0;

struct MoveOnly {
    MoveOnly() = delete;
    explicit MoveOnly(int) {
    }
    MoveOnly(MoveOnly&&) = default;
    MoveOnly(const MoveOnly&) = delete;

    ~MoveOnly() {
        ++move_deleted;
    }
    MoveOnly& operator=(MoveOnly&&) = default;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

TEST_CASE("List works with move-only type", "[List]") {
    REQUIRE(move_deleted == 0);
    {
        List<MoveOnly> l;
        REQUIRE(l.IsEmpty());
        {
            MoveOnly m{2};
            l.PushBack(std::move(m));
        }
        REQUIRE(move_deleted == 1);
        REQUIRE(l.Size() == 1);

        List<MoveOnly> l2 = std::move(l);
        REQUIRE(move_deleted == 1);

        REQUIRE(l.Size() == 0);
        REQUIRE(l2.Size() == 1);
    }
    REQUIRE(move_deleted == 2);
}

TEST_CASE("List has const methods", "[List]") {
    List<int> l;
    l.PushFront(0);
    l.PushBack(1);
    REQUIRE(l.Front() == 0);
    REQUIRE(l.Back() == 1);
    const auto& cl = l;
    REQUIRE(cl.Front() == 0);
    REQUIRE(cl.Back() == 1);
    REQUIRE(!cl.IsEmpty());
    REQUIRE(cl.Size() == 2);
}

TEST_CASE("List supports iteration", "[List]") {
    List<int> l;
    l.PushBack(1);
    l.PushBack(2);
    l.PushBack(3);
    l.PushBack(4);
    int i = 0;
    for (auto it = l.Begin(); it != l.End(); ++it) {
        REQUIRE(*it == ++i);
    }
    i = 0;
    for (int v : l) {
        REQUIRE(v == ++i);
    }
}

TEST_CASE("List behaves like a vector", "[List]") {
    constexpr int kIterations = 10000;
    constexpr int kMaxSize = 50;

    std::default_random_engine engine(42);

    List<int> list;
    std::vector<int> vector;

    std::uniform_int_distribution<int> random_int(0, 1000);
    std::uniform_int_distribution<int> random_case(0, 4);

    for (int i = 0; i < kIterations; i++) {
        switch (random_case(engine)) {
            case 0: {
                if (list.Size() == kMaxSize) {
                    break;
                }

                list.PushBack(random_int(engine));
                vector.push_back(list.Back());

                break;
            }
            case 1: {
                if (list.Size() == kMaxSize) {
                    break;
                }

                list.PushFront(random_int(engine));
                vector.insert(vector.begin(), list.Front());

                break;
            }
            case 2: {
                if (list.Size() == 0) {
                    break;
                }

                list.PopBack();
                vector.pop_back();

                break;
            }
            case 3: {
                if (list.Size() == 0) {
                    break;
                }

                list.PopFront();
                vector.erase(vector.begin());

                break;
            }
            case 4: {
                REQUIRE(list.Size() == vector.size());
                int i = 0;
                for (const auto& element : list) {
                    REQUIRE(element == vector[i++]);
                }

                break;
            }
        }
    }
}

TEST_CASE("List has a copy constructor", "[List]") {
    List<std::shared_ptr<int>> l1;
    l1.PushBack(std::make_shared<int>(0));
    l1.PushBack(std::make_shared<int>(1));
    l1.PushBack(std::make_shared<int>(2));

    List<std::shared_ptr<int>> l2{l1};
    List<std::shared_ptr<int>> l3;

    l3 = l1;

    REQUIRE(l3.Size() == 3);
    REQUIRE(l2.Size() == 3);

    REQUIRE(l1.Front().use_count() == 3);
}

TEST_CASE("List has a move constructor", "[List]") {
    List<std::unique_ptr<int>> l1;

    l1.PushBack(std::make_unique<int>(0));
    l1.PushBack(std::make_unique<int>(1));
    l1.PushBack(std::make_unique<int>(2));

    List<std::unique_ptr<int>> l2{std::move(l1)};

    REQUIRE(*l2.Front() == 0);

    REQUIRE(l1.Size() == 0);
    REQUIRE(l2.Size() == 3);

    List<std::unique_ptr<int>> l3;

    l3 = std::move(l2);

    REQUIRE(l2.Size() == 0);
    REQUIRE(l3.Size() == 3);

    REQUIRE(*l3.Front() == 0);
}

TEST_CASE("List iterators work", "[List]") {
    List<int> l;
    l.PushBack(0);
    l.PushBack(1);
    l.PushBack(2);

    auto i = l.Begin();
    REQUIRE(*i == 0);
    REQUIRE(*(++i) == 1);
    REQUIRE(*(++i) == 2);

    REQUIRE(*(i++) == 2);
    REQUIRE(i == l.End());

    REQUIRE(*(--i) == 2);
    REQUIRE(*(--i) == 1);
    REQUIRE(*(i--) == 1);

    REQUIRE(i == l.Begin());

    i++;
    REQUIRE(i == ++l.Begin());
}
