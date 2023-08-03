#include "test_type_rich.h"

#include <iostream>
#include <vector>
#include <mutex>
#include <deque>
#include <memory>

int main() {
    std::vector<VerboseTest> kek;
    kek.reserve(100);

    VerboseTest test1;
    kek.push_back(std::move(test1));
    puts("");

    VerboseTest test2;
    kek.emplace_back(std::move(test2));  // Exactly the same as the version above.

    std::deque<std::mutex> useful_emplace_back1;
    useful_emplace_back1.emplace_back();

    std::vector<std::pair<int, int>> useful_emplace_back2;
    useful_emplace_back2.emplace_back(2, 3);

    int x = 5;
    std::vector<std::unique_ptr<int>> dangerous_emplace_back;
    // dangerous_emplace_back.emplace_back(&x);  // causes a segfault
    // dangerous_emplace_back.push_back(&x);  // doesn't compile, since this constructor is explicit
    return 0;
}
