#include <iostream>
#include <memory>

void f(const std::string& kek) {
    std::cout << "lvalue reference to const overload f(" << kek << ")\n";
}

void f(std::string&& kek) {
    std::cout << "rvalue reference overload f(" << kek << ")\n";
}

void g(const std::string& kek) {
    std::cout << "lvalue reference to const overload g(" << kek << ")\n";
}

void g(const std::string&& kek) {
    std::cout << "rvalue reference to const overload g(" << kek << ")\n";
}

struct GetRef {
    template <class T>
    const T* operator()(const T& arg) {
        return &arg;
    }

    template <class T>
    T& operator()(const T&&) = delete;  // Binds to all rvalues.
};

int main() {
    std::string s1 = "hello";
    f(s1);
    f(std::move(s1));

    const std::string s2 = "darkness";
    f(s2);
    // This could be very expensive.
    f(std::move(s2));

    const std::string s3 = "my_old_friend";
    g(s3);
    // This can be overloaded correctly, if really needed, but const&& makes little sense.
    g(std::move(s3));

    // The only reasonable usage:
    GetRef kek;
    int x = 2;
    std::cout << kek(x) << "\n";
    // kek(10) wouldn't compile.
}
