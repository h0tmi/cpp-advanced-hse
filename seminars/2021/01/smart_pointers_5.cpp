#include <iostream>
#include <memory>

struct B;

struct A {
    A() {
        printf("A()\n");
    }

    ~A() {
        printf("~A()\n");
    }

    std::shared_ptr<B> ptr;
};

struct B {
    B() {
        printf("B()\n");
    }
    ~B() {
        printf("~B()\n");
    }

    std::shared_ptr<A> ptr;
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->ptr = b;
    b->ptr = a;
    printf("use_count for a = %ld\n", a.use_count());
    printf("use_count for b = %ld\n", b.use_count());
    //  b->ptr.reset();
    //  printf("use_count for a = %ld\n", a.use_count());
    //  printf("use_count for b = %ld\n", b.use_count());
}
