#include <thread>
#include <sstream>
#include <iostream>
#include <exception>
#include <chrono>

void ThrowException() {
    throw std::runtime_error("Catch this!");
}

int main() {
    std::thread worker(ThrowException);
    std::this_thread::sleep_for(std::chrono::seconds(30));

    worker.join();
    return 0;
}
