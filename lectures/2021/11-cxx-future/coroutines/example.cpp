#include <iostream>
#include <coroutine>
#include "cppcoro/include/cppcoro/generator.hpp"

cppcoro::generator<int> foo() {
  int i = 0;
  while (true) {
    i += 2;
    co_yield i;
  }
}

int main() {
  int x = 0;
  for (const auto& el : foo()) {
    std::cout << el << '\n';
    x++;
    if (x > 10) break;
  }
}
