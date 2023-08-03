#include <typeinfo>
#include <iostream>

// No polymorphism -> no RTTI checks are done -> answer is always A
struct A {};
struct B : A {};

std::type_info const& GiveMeType(A&& a) {
    return typeid(a);
}

// do not forget to demangle: ./a.out | c++filt -t

int main() {
    const auto& info = GiveMeType(B{});
    std::cout << info.name() << std::endl;

    // no evaluation!
    std::cout << typeid(*((B*)nullptr)).name() << std::endl;

    // allowed since C++20, not working on Apple Clang
    // look here https://godbolt.org/z/hc1Ph3fT1
    // no evaluation too!
    std::cout << typeid([]() {
                     int* null_pointer = nullptr;
                     *null_pointer = 1312;
                 }).name()
              << std::endl;
}
