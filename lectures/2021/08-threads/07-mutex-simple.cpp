#include <thread>
#include <sstream>
#include <iostream>
#include <mutex>

std::mutex OutputMutex;

void WriteHelloWorld() {
    std::unique_lock<std::mutex> guard(OutputMutex);
    std::cerr << "[tid:";
    std::cerr << std::this_thread::get_id();
    std::cerr << "] Hello World";
    std::cerr << std::endl;
}

int main() {
    std::thread first(WriteHelloWorld);
    std::thread second(WriteHelloWorld);

    first.join();
    second.join();
    return 0;
}
