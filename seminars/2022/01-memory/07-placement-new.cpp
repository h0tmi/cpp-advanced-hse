#include "test_type.h"

#include <iostream>
#include <cassert>

// A global buffer that we want to fill with a string later.
alignas(std::string) char buf[sizeof(std::string)];
// This is usually const-initialized, we can add the constinit keyword to force this in C++20.
// This means that in-practice we don't have to deal with SIOF here.

void FillBuf(const std::string& value) {
    auto ptr = new (buf) std::string{value};
    puts(ptr->c_str());
}

int main() {
    std::allocator<Test> allocator{};
    Test* raw_ptr = allocator.allocate(1);
    // We got raw, uninitialized memory, what do we do now?
    // puts(raw_ptr->Get().c_str()); // UB.
    // We need to construct an object at a specific address.
    Test* ptr = new (raw_ptr) Test("kek");
    assert(ptr == raw_ptr);
    puts(ptr->Get().c_str());
    // If the object has a non-trivial destructor, we need to call it manually:
    ptr->~Test();
    allocator.deallocate(ptr, 1);
    // If the destructor has no "side effects" we are not *required* to call it.
    // This allows making more efficient memory pools and so on.

    // With great power comes great responsibility, we have to align memory ourselves:
    char memory[5];
    printf("%p\n", memory);
    auto ptr2 = new (memory) int{42};  // This is UB.

    alignas(int) char aligned_memory[5];
    auto ptr3 = new (memory) int{42};  // This is fine.

    FillBuf("kek");
    FillBuf("double kek");
}