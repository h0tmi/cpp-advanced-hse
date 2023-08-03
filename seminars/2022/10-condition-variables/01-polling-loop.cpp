#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<bool> is_ready(false);

void foo() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    is_ready.store(true);
}

int main() {
    std::thread t(foo);
    while (!is_ready.load()) {
        std::this_thread::yield();
    }
    t.join();
}
