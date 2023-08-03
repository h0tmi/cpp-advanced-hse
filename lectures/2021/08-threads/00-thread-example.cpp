#include <thread>
#include <sstream>
#include <iostream>

void WriteHelloWorld() {
    std::stringstream ss;
    ss << "[tid:" << std::this_thread::get_id() << "] Hello World" << std::endl;
    std::cout << ss.str();
}

int main() {
    std::thread first(WriteHelloWorld);
    std::thread second(WriteHelloWorld);

    first.join();
    second.join();
    return 0;
}
