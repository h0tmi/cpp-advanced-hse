#include "test_type.h"

#include <iostream>
#include <memory>

// Resource* create();
void* create() {
    puts("Allocating");
    return malloc(5);
}

// void destroy(Resource* ptr);
void destroy(void* ptr) {
    puts("Deallocating");
    free(ptr);
}

void AnotherScope(std::unique_ptr<Test> ptr) {
    std::cout << "moved into another scope " << ptr->Get() << "\n";
}

int main() {
    auto ptr1 = std::unique_ptr<Test>(new Test("1"));
    // We can skip writing new altogether.
    auto ptr2 = std::make_unique<Test>("2");
    // Fewer issues with exception safety.
    // foo(std::unique_ptr<T>(new T), otherFunction()) -- could cause memory leaks until C++ 17.
    AnotherScope(std::move(ptr2));
    // What were the choices before move semantics were introduced?
    // call ptr.release() and wrap it in another scoped_ptr later on (easy to make mistakes)
    // auto_ptr, COAP...

    // Specialization for arrays.
    auto ptr3 = std::make_unique<Test[]>(2);
    // unique_ptr can be used for managing resources borrowed from C.
    std::unique_ptr<void, decltype(&destroy)> ptr4(create(), destroy);
    // A more "practical" example.
    std::unique_ptr<FILE, decltype(&fclose)> fp(fopen("test.txt", "w"), fclose);
}
