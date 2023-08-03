#include <iostream>
#include <syncstream>
#include <thread>

void RacyWorker(int idx) {
    std::cerr << "Thread " << idx << " (id=" << std::this_thread::get_id() << ")" << std::endl;
    // Possible output:
    // Thread 1 (id=Thread 140691653121600)0 (id=140691661514304)
    //
}

void SyncedWorker(int idx) {
    std::osyncstream{std::cerr} << "Thread " << idx << " (id=" << std::this_thread::get_id() << ")"
                                << std::endl;
}

int main() {
    {
        std::cerr << "Racy output:" << std::endl;
        std::jthread t1{RacyWorker, 0};
        std::jthread t2{RacyWorker, 1};
    }

    {
        std::cerr << "Synced output:" << std::endl;
        std::jthread t1{SyncedWorker, 0};
        std::jthread t2{SyncedWorker, 1};
    }
}
