#include <catch.hpp>

#include <string>
#include <vector>

#include <intrusive_list.h>

struct Item : public ListHook {
    explicit Item(int i) : i(i), blob("abcdefgh") {
        blob += std::to_string(i);
    }

    int i;
    std::string blob;  // put nontrivial field here, hopefully incorrect code
                       // will hit double free

    Item(const Item&) = delete;
    Item(Item&&) = delete;
    Item& operator=(const Item&) = delete;
    Item& operator=(Item&&) = delete;
};

TEST_CASE("Empty list works", "[IntrusiveList]") {
    List<Item> l1;
    REQUIRE(l1.IsEmpty());
    REQUIRE(l1.Size() == 0);

    Item i1(1);
    REQUIRE(!i1.IsLinked());
}

TEST_CASE("Back works", "[IntrusiveList]") {
    List<Item> l;
    Item i1(1), i2(2), i3(3);

    l.PushBack(&i1);
    REQUIRE(l.Back().i == 1);

    l.PushBack(&i2);
    REQUIRE(l.Back().i == 2);
    REQUIRE(l.Size() == 2);
    REQUIRE(!l.IsEmpty());

    REQUIRE(i1.IsLinked());
    REQUIRE(i2.IsLinked());

    l.PopBack();
    REQUIRE(!l.IsEmpty());
    REQUIRE(l.Size() == 1);
    REQUIRE(l.Back().i == 1);

    l.PopBack();
    REQUIRE(l.IsEmpty());
    REQUIRE(l.Size() == 0);

    REQUIRE(!i1.IsLinked());
    REQUIRE(!i2.IsLinked());
}

TEST_CASE("Front works", "[IntrusiveList]") {
    List<Item> l;
    Item i1(1), i2(2), i3(3);

    l.PushFront(&i1);
    REQUIRE(l.Front().i == 1);
    REQUIRE(l.Back().i == 1);

    l.PushFront(&i2);
    REQUIRE(l.Back().i == 1);
    REQUIRE(l.Front().i == 2);
    REQUIRE(l.Size() == 2);
    REQUIRE(!l.IsEmpty());

    l.PopFront();
    REQUIRE(!l.IsEmpty());
    REQUIRE(l.Size() == 1);
    REQUIRE(l.Front().i == 1);

    l.PopBack();
    REQUIRE(l.IsEmpty());
    REQUIRE(l.Size() == 0);
}

TEST_CASE("Const methods", "[IntrusiveList]") {
    List<Item> l;
    Item i1(1), i2(2), i3(3);
    l.PushFront(&i1);
    l.PushBack(&i2);
    REQUIRE(l.Front().i == 1);
    REQUIRE(l.Back().i == 2);

    const auto& cl = l;
    REQUIRE(cl.Front().i == 1);
    REQUIRE(cl.Back().i == 2);
    REQUIRE(!cl.IsEmpty());
    REQUIRE(cl.Size() == 2);
}

TEST_CASE("Iteration", "[IntrusiveList]") {
    List<Item> l;
    Item i1(1), i2(2), i3(3);

    l.PushBack(&i1);
    l.PushBack(&i2);
    l.PushBack(&i3);
    int i = 0;
    for (auto it = l.Begin(); it != l.End(); ++it) {
        REQUIRE(it->i == ++i);
    }

    i = 0;
    for (Item& v : l) {
        REQUIRE(v.i == ++i);
    }
}

TEST_CASE("Unlink works", "[IntrusiveList]") {
    List<Item> l;
    Item i1(1), i2(2), i3(3);

    l.PushBack(&i1);
    l.PushBack(&i2);
    l.PushBack(&i3);

    i2.Unlink();
    REQUIRE(l.Front().i == 1);
    REQUIRE(l.Back().i == 3);
    REQUIRE(l.Size() == 2);
}

TEST_CASE("Destructor works", "[IntrusiveList]") {
    Item i1(1), i2(2), i3(3);

    {
        List<Item> l;
        l.PushBack(&i1);
        l.PushBack(&i2);
        l.PushBack(&i3);
    }

    REQUIRE(!i1.IsLinked());
    REQUIRE(!i2.IsLinked());
    REQUIRE(!i3.IsLinked());
}

TEST_CASE("Item destructor works", "[IntrusiveList]") {
    List<Item> l;

    {
        Item i1(1), i2(2), i3(3);
        l.PushBack(&i1);
        l.PushBack(&i2);
        l.PushBack(&i3);
    }

    REQUIRE(0 == l.Size());
}

TEST_CASE("IteratorTo works", "[IntrusiveList]") {
    Item i1(1), i2(2), i3(3);
    List<Item> l1;

    l1.PushBack(&i1);
    l1.PushBack(&i2);
    l1.PushBack(&i3);

    auto it = l1.IteratorTo(&i2);
    REQUIRE(it->i == 2);

    it++;
    REQUIRE(it == l1.IteratorTo(&i3));

    it++;
    REQUIRE(it == l1.End());
}

TEST_CASE("Move works", "[IntrusiveList]") {
    Item i1(1), i2(2), i3(3);
    List<Item> l1;

    l1.PushBack(&i1);
    l1.PushBack(&i2);
    l1.PushBack(&i3);

    List<Item> l2(std::move(l1));

    REQUIRE(!l2.IsEmpty());
    REQUIRE(l1.IsEmpty());

    l1 = std::move(l2);

    REQUIRE(!l1.IsEmpty());
    REQUIRE(l2.IsEmpty());
}
