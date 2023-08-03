#include <thread>
#include <iostream>
#include <string>
#include <chrono>

void BackgroundWork(std::stop_token token) {
    while (!token.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cerr << ".";
    }
}

int main() {
    std::jthread background_work(BackgroundWork);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}

