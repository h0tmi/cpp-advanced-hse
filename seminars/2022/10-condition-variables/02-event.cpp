#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

class Event {
public:
    void Signal() {
        {
            std::lock_guard lock{mtx_};
            ready_ = true;
        }
        // We do not hold the lock here in order to increase throughput
        cv_.notify_all();
    }

    void Wait() {
        std::unique_lock lock{mtx_};
        cv_.wait(lock, [this] { return ready_; });
    }

    bool Ready() {
        std::lock_guard lock{mtx_};
        return ready_;
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    bool ready_ = false;
};

int main() {
    Event event;

    std::thread worker{[&event] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        event.Signal();
    }};

    std::cerr << "Waiting for event...\n";
    event.Wait();
    std::cerr << "Done!\n";

    worker.join();
}
