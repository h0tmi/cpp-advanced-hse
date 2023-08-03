#include <memory>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////

class Pinned : public std::enable_shared_from_this<Pinned> {
public:
    Pinned() {
        std::cout << "Pinned()" << std::endl;
    }

    Pinned(const Pinned&) = delete;
    Pinned(const Pinned&&) = delete;

    Pinned& operator=(const Pinned&) = delete;
    Pinned& operator=(const Pinned&&) = delete;

    ~Pinned() {
        std::cout << "~Pinned()" << std::endl;
    }
};

///////////////////////////////////////////////////////////////////////////////

void WrongSharedPtrCopy() {
    auto pinned = new Pinned();

    std::shared_ptr<Pinned> p1(pinned);

    // Wrong! p2 won't know about p1
    std::shared_ptr<Pinned> p2(pinned);
}

void WrongESFTUsage1() {
    Pinned pinned;
    auto p = pinned.shared_from_this();  // throws std::bad_weak_ptr
}

void WrongESFTUsage2() {
    auto pinned = new Pinned();
    auto p = pinned->shared_from_this();  // throws std::bad_weak_ptr
}

///////////////////////////////////////////////////////////////////////////////

int main() {
    // WrongSharedPtrCopy();
    // WrongESFTUsage1();
    // WrongESFTUsage1();

    // You can create object via std::make_shared...
    auto p1 = std::make_shared<Pinned>();

    // ...or pass existing raw pointer manually, this is still correct
    /*
    auto pinned = new Pinned();
    auto p1 = std::shared_ptr<Pinned>(pinned);
    */

    // Then you can obtain shared_ptr directly from Pinned*
    auto p2 = p1.get()->shared_from_this();

    std::cout << "Resetting source pointer..." << std::endl;
    p1.reset();
    std::cout << "Done reset, but object is still alive" << std::endl;
}
