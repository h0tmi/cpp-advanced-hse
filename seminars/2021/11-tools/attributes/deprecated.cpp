#include <stdexcept>
#include <iostream>
#include <cstdlib>

[[deprecated("stop doing this")]] void old_function() {
}

int main() {
    old_function();

    int x = 0;
    // [[deprecated]] int x = 0;
    x++;
}