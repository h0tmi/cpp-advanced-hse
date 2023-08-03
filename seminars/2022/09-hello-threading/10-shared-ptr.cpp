#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <shared_mutex>
#include <utility>
#include <iostream>
#include <syncstream>

#define LOG            \
    std::osyncstream { \
        std::cerr      \
    }

class SharedResource {
public:
    SharedResource(size_t gen) : generation_{gen} {
    }

    size_t Generation() const {
        return generation_;
    }

    ~SharedResource() {
        LOG << "Destroying resource " << Generation() << " from thread "
            << std::this_thread::get_id() << std::endl;
    }

private:
    size_t generation_ = 0;
};

using SharedResourcePtr = std::shared_ptr<SharedResource>;

class ResourceRegistry {
public:
    void Store(SharedResourcePtr resource) {
        std::lock_guard guard{lock_};
        current_ = std::move(resource);
    }

    SharedResourcePtr Load() {
        std::shared_lock guard{lock_};  // Required
        return current_;
    }

private:
    std::shared_mutex lock_;
    SharedResourcePtr current_;
};

void Updater(ResourceRegistry* registry) {
    LOG << "Start updater at thread " << std::this_thread::get_id() << std::endl;

    for (size_t seq = 0; true; ++seq) {
        registry->Store(std::make_shared<SharedResource>(seq));
        LOG << "Stored new resource " << seq << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Reader(ResourceRegistry* registry, int i) {
    LOG << "Start reader " << i << " at thread " << std::this_thread::get_id() << std::endl;

    // Generator for timeouts
    std::mt19937 mt{42u * i};
    std::uniform_int_distribution<int> dist{500, 1200};

    for (;;) {
        auto resource = registry->Load();

        if (resource) {
            LOG << "Reader " << i << " loaded resource " << resource->Generation() << std::endl;
        } else {
            LOG << "Reader " << i << " loaded empty resource" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(dist(mt)));
    }
}

int main() {
    ResourceRegistry registry;
    std::jthread updater{Updater, &registry};
    std::jthread reader1{Reader, &registry, 0};
    std::jthread reader2{Reader, &registry, 1};
}
