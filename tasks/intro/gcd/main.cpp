#include <numeric>
#include <iostream>

int Gcd(int a, int b) {
    while (a * b) {
        int rem = a % b;
        a = b;
        b = rem;
    }
    return std::max(a, b);
}

void TryRead(int *x) {
    if (!(std::cin >> *x)) {
        std::cout << "Fail on reading" << std::endl;
        exit(0);
    }
}

int main() {
    int a, b;
    TryRead(&a);
    TryRead(&b);
    if (a <= 0 || b <= 0) {
        std::cout << "Numbers should be positive" << std::endl;
        return 0;
    }

    if (Gcd(a, b) != std::gcd(a, b)) {
        std::cout << "Shit happens" << std::endl;
        return 1;
    } else {
        std::cout << "Try again..." << std::endl;
    }

    return 0;
}
