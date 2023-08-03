#include <thread>
#include <atomic>
#include <cstdint>
#include <vector>
#include <memory>

void Boom() {
    static std::vector<std::shared_ptr<std::atomic<uint64_t>>> Singleton{
        std::make_shared<std::atomic<uint64_t>>(0)
    };

    Singleton[0]->fetch_add(1);
}

void Background() {
    while (true) {
        Boom();
    }
}

int main() {
    std::thread(Background).detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
