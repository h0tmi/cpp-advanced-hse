#include <iostream>

int f(int x) {
    switch (x) {
        case 1:
            std::cout << "1\n";
            [[fallthrough]];  // compiler may warn without fallthrough
        case 2:
            std::cout << "2\n";
            return 1;
    }
    return 0;
}

int main() {
    f(1);
}