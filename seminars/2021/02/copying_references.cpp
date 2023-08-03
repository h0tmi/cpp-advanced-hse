#include <iostream>
#include <map>

template <class A, class B>
struct MyPair {
    A first;
    B second;
};

template <class A, class B>
struct MyPairWithCopyAssignment {
    MyPairWithCopyAssignment& operator=(const MyPairWithCopyAssignment& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    A first;
    B second;
};

template <class A, class B, bool holds_references>
struct BetterPairBase;

template <class A, class B>
struct BetterPairBase<A, B, false> {
    A first;
    B second;

    //  BetterPairBase(const A& first_, const B& second_): first(second_), second(second_) {
    //  }

    BetterPairBase& operator=(const BetterPairBase& other) = default;
};

template <class A, class B>
struct BetterPairBase<A, B, true> {
    A first;
    B second;

    BetterPairBase& operator=(const BetterPairBase& other) {
        first = other.first;
        second = other.second;
        return *this;
    }
};

template <class A, class B>
struct BetterPair : BetterPairBase<A, B, std::is_reference_v<A> || std::is_reference_v<B>> {
    using Base = BetterPairBase<A, B, std::is_reference_v<A> || std::is_reference_v<B>>;

    //  template<class U, class V>
    //  BetterPair(U&& first, V&& second): Base(std::forward<U>(first), std::forward<V>(second)) {
    //  }
};

int main() {
    // References and pointers are vastly different in copying semantics:
    int x = 1;
    int y = 2;
    int& rx = x;
    int& ry = y;
    rx = ry;  // Changes the value of x (!)
    printf("x = %d, y = %d\n", x, y);

    int z = 33;
    int t = 44;
    int* px = &x;
    int* py = &y;
    px = py;  // Now px just points to y.
    assert(px == &y);
    printf("z = %d, t = %d\n", z, t);

    MyPair<int&, int&> pair1 = {x, y};
    MyPair<int&, int&> pair2 = {z, t};
    // Ill-formed. Copy-assignment operator is implicitly-deleted if class holds reference-members.
    // pair2 = pair1;
    static_assert(std::is_trivially_copyable_v<MyPair<int&, int&>>);

    x = 1;
    y = 2;
    z = 3;
    t = 4;
    MyPairWithCopyAssignment<int&, int&> pair3 = {x, y};
    MyPairWithCopyAssignment<int&, int&> pair4 = {z, t};
    pair4 = pair3;
    printf("x = %d, y = %d, z = %d, t = %d\n", x, y, z, t);
    // That's a problem though:
    static_assert(!std::is_trivially_copyable_v<MyPairWithCopyAssignment<int, int>>);
    // We don't expect this for references, but for other types we lose things like std::memcpy.

    x = 5;
    y = 6;
    z = 7;
    t = 8;
    BetterPair<int&, int&> pair5 = {x, y};
    BetterPair<int&, int&> pair6 = {z, t};
    pair6 = pair5;
    printf("x = %d, y = %d, z = %d, t = %d\n", x, y, z, t);
    static_assert(std::is_trivially_copyable_v<BetterPair<int, int>>);  // What we wanted.
    static_assert(!std::is_trivially_copyable_v<BetterPair<int&, int>>);

    // Why do we even want to hold references in std::pair?
    std::map<int, int> kek;
    std::map<int, int>::iterator iterator;
    bool inserted;
    std::tie(iterator, inserted) = kek.insert({3, 2});
    printf("%d %d\n", iterator->second, inserted);
}