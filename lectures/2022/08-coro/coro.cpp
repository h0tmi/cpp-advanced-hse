#include "resumable.h"
#include <iostream>


resumable hello_world() {
    std::cout << "Hello, ";
    co_await std::experimental::suspend_always{};
    std::cout << "world!\n";
}

int main() {
    resumable coro = hello_world();
    coro.resume();
    std::cout << "coroutine ";
    coro.resume();
    coro.resume();
    coro.resume();
    coro.resume();
    coro.resume();
    coro.resume();
    coro.resume();
    coro.resume();
}
