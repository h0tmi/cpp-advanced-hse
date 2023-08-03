#include <iostream>
#include <string_view>

// Objects with size <= 16 should be passed by value on x86.
// Why?
// Pass by reference: passes the reference through a register, we need to dereference to get the
// actual value of the object.
// Pass by value: passes the value using two registers, no additional indirection.
void PrintFirstTwoChars(std::string_view view) {
    std::cout << view.substr(0, 2) << "\n";
}

// We can try to demonstrate this with assembly in real-time during the seminar.
// Other architectures can have different calling conventions.

struct BigNoNo {
    std::string_view str;
};

int main() {
    std::string s{"kek"};
    std::string_view sv = s;
    PrintFirstTwoChars(sv);

    // Be careful about lifetimes, the underlying object has to outlive the view!
    BigNoNo bad = {std::string{"hello"}};
}