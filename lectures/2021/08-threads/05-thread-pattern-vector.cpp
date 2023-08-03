#include <thread>
#include <vector>
#include <sstream>
#include <iostream>

int main() {
    std::vector<int> work = {1, 2, 3, 4};

    std::vector<std::thread> workers;
    for (size_t i = 0; i < work.size(); i++) {
        workers.emplace_back([&work, i] {
            work[i] = work[i] * work[i];
        });
    }

    for (auto& t : workers) {
        t.join();
    }
    return 0;
}
