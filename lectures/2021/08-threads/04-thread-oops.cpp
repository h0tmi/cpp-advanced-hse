#include <thread>
#include <sstream>
#include <iostream>
#include <exception>

void DoWork() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    std::thread worker(DoWork);
    try {

        throw std::runtime_error("Other part of the program failed");

    } catch (const std::exception& ex) {
        worker.join(); // same problem, as delete + naked pointer
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
