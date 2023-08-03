import hello;
import say;
import foo;

#include <iostream>
#include <vector>

int main() {
    hello::SayHello();
    Say{}.Hello<sizeof(Say)>();
    auto foo = Foo<std::vector<int>>{};
    std::cout << foo.Hello() << std::endl;
    return 0;
}
