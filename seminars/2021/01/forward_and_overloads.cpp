#include <iostream>

void f(int& x) {
    std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(int&& x) {
    std::cout << "rvalue reference overload f(" << x << ")\n";
}

template <typename T>
T&& MyWrongForward(T&& arg) {
    puts(__PRETTY_FUNCTION__);
    return static_cast<T&&>(arg);
}

template <typename T>
void Bar(T&& arg) {
    f(MyWrongForward(arg));
}

template <typename T>
void Foo(T&& arg) {
    // arg is an lvalue in the context of the std::forward call, so writing std::forward(arg)
    // is semantically useless, there is no way to know what arg really is.
    // f(MyWrongForward(arg)) results in T = int& in all cases (of ints), so you would always get an
    // lvalue in return.
    f(std::forward<T>(arg));
}

template <typename T>
void FooBar(T&& arg) {
    // If arg is a rvalue reference (int&&), then T is deduced as int.
    // If arg is an lvalue reference (int&), then T is deduced as int&. (special rule)
    // In this case std::forward is just static_cast<T&&>(arg), see collapsing rules.
    f(static_cast<T&&>(arg));
}

int main() {
    // Rvalue reference variables are lvalues when used in expressions.
    int&& x = 1;
    f(x);             // calls f(int& x)
    f(std::move(x));  // calls f(int&& x)
    puts("");

    int&& y = 2;
    Foo(y);             // calls f(int& x)
    Foo(std::move(y));  // calls f(int&& x)
    puts("");

    int&& z = 3;
    Bar(z);             // calls f(int& x)
    Bar(std::move(z));  // calls f(int& x)
    puts("");

    int&& t = 4;
    FooBar(t);             // calls f(int& x)
    FooBar(std::move(t));  // calls f(int&& x)
}
