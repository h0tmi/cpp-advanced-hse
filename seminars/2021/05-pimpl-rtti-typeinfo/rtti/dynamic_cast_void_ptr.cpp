#include <cassert>

struct B {
    int a;
    int b;
    virtual ~B() = default;
};

struct C {
    int a;
    int b;
    virtual ~C() = default;
};

struct D : C, B {
    int c;
    int d;
};

int main() {
    D* ptrd = new D;
    B* ptrb = ptrd;
    C* ptrc = ptrd;
    // dynamic_cast<void*>(ptr) casts to the most derived object
    assert(dynamic_cast<void*>(ptrb) == ptrd);
    assert(dynamic_cast<void*>(ptrc) == ptrd);
    delete ptrd;
}
