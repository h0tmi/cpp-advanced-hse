#include <iostream>

template <typename T>
void f(T t) {
    puts(__PRETTY_FUNCTION__);
}

int main() {
    f(0);

    // Resolves to an integer, often just a macro for 0.
    // Can be a problem given multiple overloads.
    f(NULL);

    // Has type void*.
    f(static_cast<void*>(0));
    // We would want to write T* ptr = null pointer, but casting void* to any T* is forbidden by
    // C++. int* kek = static_cast<void*>(0);

    // Has its own separate type std::nullptr_t.
    f(nullptr);
    // Useful for things like this:
    // class basic_string {
    // public:
    //   basic_string(const char* ptr);
    //   basic_string(std::nullptr_t) = delete;
    // };
}