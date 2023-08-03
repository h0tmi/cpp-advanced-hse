// compile with -Wthread-safety flag

#include <mutex>
#include <vector>

class __attribute__((capability("mutex"))) Mutex {
private:
    std::mutex std_mutex;

public:
    void Lock() __attribute__((exclusive_lock_function)) {
        std_mutex.lock();
    }

    void Unlock() __attribute__((unlock_function)) {
        std_mutex.unlock();
    }
};

int main() {
    std::vector<Mutex> mutexes(10);
    for (int i = 0; i < 10; ++i)
        mutexes[i].Lock();
    for (int i = 0; i < 10; ++i)
        mutexes[i].Unlock();
}
