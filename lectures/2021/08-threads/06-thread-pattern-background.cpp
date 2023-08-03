#include <thread>
#include <sstream>
#include <iostream>

void LogProgress() {
    std::cerr << ".";
}

void BackgroundWork() {
    // No try-catch here.
    // Uncaught exception in background (or any) thread would kill the whole process.
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        LogProgress();
    }
}

void DoMainWork() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main() {
    std::thread background_work(BackgroundWork);
    background_work.detach();

    DoMainWork();
    return 0;
}
