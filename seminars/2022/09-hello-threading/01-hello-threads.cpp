#include <iostream>
#include <sstream>
#include <syncstream>
#include <thread>

std::string ToString(std::thread::id id) {
    std::ostringstream ss;
    ss << id;
    return ss.str();
}

void Worker(int idx) {
    // printf is thread-safe
    printf("Thread %d (id=%s)\n", idx, ToString(std::this_thread::get_id()).c_str());
}

int main() {
    std::thread t1{Worker, 0};
    std::jthread t2{Worker, 1};
    t1.join();
    // No need to join t2
}
