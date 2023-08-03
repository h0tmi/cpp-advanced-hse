#include <iostream>
#include <vector>

struct MoveAndCopy {
    MoveAndCopy() = default;
    MoveAndCopy(const MoveAndCopy&) {
        puts("copying");
    }
    MoveAndCopy& operator=(const MoveAndCopy&) = delete;
    MoveAndCopy(MoveAndCopy&&) {
        puts("moving");
    }
    MoveAndCopy& operator=(MoveAndCopy&&) = default;
};

int main() {
    std::vector<MoveAndCopy> kek(2);
    kek.emplace_back();  // calls copy-constructor unless move-constructor is marked noexcept!
                         // static_assert(std::is_nothrow_move_constructible_v<MoveAndCopy>);

    // Why?
    // If the move-constructor throws, reallocation would violate strong exceptions safety
    // guarantees: an exception in the midst of reallocating can leave some old elements in a
    // moved-from state, and it is not possible to move them back, since that could also throw. If
    // we are copying, we can simply destroy the newly created copies, as the originals were kept
    // intact.

    // As a result, the order of precedence on reallocation (and other similar operations) is:
    //   1. noexcept move-constructor
    //   2. copy-constructor
    //   3. move-constructor, but without any guarantees
}
