#include <mutex>
#include <shared_mutex>
#include <thread>
#include <iostream>

class WrongCounter {
public:
    void Update(int value) {
        std::unique_lock guard(m_);
        sum_ += value;
        count_ += 1;
    }

    int GetAvg() const {
        std::shared_lock guard(m_);
        return sum_ / count_;
    }

private:
    int64_t sum_ = 0;
    size_t count_ = 0;
    mutable std::shared_mutex m_;
};


int main() {
    WrongCounter c;

    std::thread t1([&c] {
        for (int i = 0; i < 1024; ++i) {
            c.Update(1000000);
        }
    });

    std::thread t2([&c] {
        for (int i = 0; i < 1024; ++i) {
            c.Update(2000000);
        }
    });

    t1.join();
    t2.join();

    std::cout << c.GetAvg() << std::endl;;

    return 0;
}
