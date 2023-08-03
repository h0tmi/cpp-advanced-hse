#include <iomanip>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>

static thread_local std::string current_thread_name;

void PrintThreadName() {
    std::osyncstream{std::cout} << "Current thread name: " << current_thread_name << std::endl;
}

void Run(const std::string& name) {
    current_thread_name = name;

    PrintThreadName();
}

int main() {
    std::jthread th1{Run, "Thread 1"};
    std::jthread th2{Run, "Thread 2"};
    std::jthread th3{Run, "Thread 3"};
}
