#include <atomic>
#include <mutex>
#include <thread>

// Naive test and set (TAS) spinlock
// Not production ready, do not use this
class SpinLock {
    enum EState {
        Unlocked,
        Locked,
    };

public:
    // NOLINTNEXTLINE
    void lock() {
        // TODO: Implement TTAS from Intel Performance Manual
        while (locked_.exchange(Locked) == Locked) {
            // TODO: tell CPU or OS that we are spinning
            // asm("pause") / std::this_thread::yield() after N failed iterations
        }

        // On last iteration locked_ was EState::Unlocked
        // So we own mutex now
    }

    // NOLINTNEXTLINE
    void unlock() {
        locked_.store(Unlocked);
    }

private:
    std::atomic<EState> locked_ = Unlocked;
};

#include <iostream>

int main() {
    SpinLock lock;
    auto greeter = [&lock](int tid) {
        std::lock_guard guard{lock};
        std::cout << "Hello "
                  << "from "
                  << "thread " << tid << std::endl;
    };

    std::jthread th[4]{
        std::jthread{greeter, 0},
        std::jthread{greeter, 1},
        std::jthread{greeter, 2},
        std::jthread{greeter, 3},
    };
}
