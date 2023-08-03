#include "test_type.h"

#include <iostream>
#include <vector>

Test GetTest(std::string name) {
    return Test{std::move(name)};
}

// No lifetime extension here.
const Test& Keker() {
    return Test{"ex3"};
}

struct MyStruct {
    int GetByValue() const {
        return x;
    }

    const int& GetByRef() const {
        return x;
    }

    const std::vector<int>& GetThings() const {
        return things;
    }

    int x;
    std::vector<int> things{1, 5, 4, 3};
};

MyStruct GetMyStruct() {
    return MyStruct{};
}

int main() {
    // Temporary objects are destroyed as the last step in evaluating their containing expression.
    puts(std::string{"kek"}.c_str());

    // Lifetime extension for temporary objects.
    int&& ex1 = 10;
    const int& ex2 = 10;
    // int& ex3 = 10; -- makes no sense, doesn't work, mentioned in the lecture.

    Test&& no_danger = GetTest("ex1");          // Fine, lifetime is extended.
    Test&& danger = std::move(GetTest("ex2"));  // Dangling reference.
    // Test&& (move just casts) can practically be anything,
    // whereas the GetTest returns a "pure" rvalue.
    std::cout << danger.Get() << "\n";  // UB.

    const Test& ex3 = Keker();  // This is also a dangling reference.
    // puts(ex3.Get()); -- UB.

    // Ok.
    const auto& field1 = GetMyStruct().GetByValue();
    // UB.
    const auto& field2 = GetMyStruct().GetByRef();
    std::cout << field2 << "\n";

    // This is also UB.
    for (int val : GetMyStruct().GetThings()) {
        std::cout << val << "\n";
    }
}
