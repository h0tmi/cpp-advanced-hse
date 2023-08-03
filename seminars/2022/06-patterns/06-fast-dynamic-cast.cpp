#include <typeinfo>
#include <iostream>

template <typename Dest, typename Src>
Dest* FastDynamicCast(Src* src) {
    if (typeid(*src) == typeid(Dest)) {
        return static_cast<Dest*>(src);
    }
    throw std::bad_cast();
}

struct A {
    virtual ~A() = default;
};

struct B : A {};
struct C : B {};

int main() {
    A* a = new B;
    FastDynamicCast<B>(a);  // ok

    try {
        A* a2 = new C;           // now we create the C object
        FastDynamicCast<B>(a2);  // uncaught exception
    } catch (const std::bad_cast& e) {
        std::cout << e.what() << "\n";
    }
}
