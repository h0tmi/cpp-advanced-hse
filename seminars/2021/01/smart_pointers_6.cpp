#include <cstdio>
#include <cstdlib>
#include <new>
#include <memory>

void* operator new(size_t size) {
    if (void* ptr = ::malloc(size)) {
        printf("new %zu = %p\n", size, ptr);
        return ptr;
    }
    throw std::bad_alloc{};
}

void operator delete(void* p) noexcept {
    printf("delete %p\n", p);
    ::free(p);
}

struct Huge {
    char arr[128];
};

void test_shared() {
    puts("test shared");
    std::shared_ptr<Huge> ptr{new Huge};
    std::weak_ptr<Huge> weak = ptr;
    ptr.reset();
    printf("weak pointer is still alive\n");
}

void test_make_shared() {
    puts("test make_shared");
    std::shared_ptr<Huge> ptr = std::make_shared<Huge>();
    std::weak_ptr<Huge> weak = ptr;
    ptr.reset();
    printf("weak pointer is still alive\n");
}

int main() {
    test_shared();
    puts("");
    test_make_shared();
}
