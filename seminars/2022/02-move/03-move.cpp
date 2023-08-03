#include <iostream>
#include <memory>

void F(const std::string& kek) {
    std::cout << "lvalue reference to const overload f(" << kek << ")\n";
}

void F(std::string&& kek) {
    std::cout << "rvalue reference overload f(" << kek << ")\n";
}

void G(const std::string& kek) {
    std::cout << "lvalue reference to const overload g(" << kek << ")\n";
}

void G(const std::string&& kek) {
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
    F(s1);
    F(std::move(s1));

    const std::string s2 = "darkness";
    F(s2);
    // This could be very expensive.
    F(std::move(s2));

    const std::string s3 = "my_old_friend";
    G(s3);
    // This can be overloaded correctly, if really needed, but const&& makes little sense.
    G(std::move(s3));

    // The only reasonable usage:
    GetRef kek;
    int x = 2;
    std::cout << kek(x) << "\n";
    // kek(10) wouldn't compile.
}
