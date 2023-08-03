#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

bool is_ready(false);
std::mutex m;
std::condition_variable cv;

void foo() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::unique_lock<std::mutex> lk(m);
    is_ready = true;
    cv.notify_one();
}

int main() {
    std::thread t(foo);
    std::unique_lock<std::mutex> lk(m);
    while (!is_ready) {
        cv.wait(lk);
        if (!is_ready) {
            std::cout << "Spurious wake up!\n";
        }
    }
    t.join();
}
