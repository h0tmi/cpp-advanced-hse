#include <thread>
#include <sstream>
#include <iostream>

void WriteHelloWorld() {
    std::cout << "[tid:" << std::this_thread::get_id() << "] Hello World" << std::endl;
}

int main() {
    std::thread first(WriteHelloWorld);
    std::thread second(WriteHelloWorld);

    first.join();
    second.join();
    return 0;
}
