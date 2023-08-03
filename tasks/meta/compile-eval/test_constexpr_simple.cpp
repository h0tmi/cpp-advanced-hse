#include <hash.h>
#include <another_pow.h>

void test_pow() {
    static_assert(pow(2, 2) == 4, "fail");
    static_assert(pow(3, 4) == 81, "fail");
    static_assert(pow(1, 10) == 1, "fail");
    static_assert(pow(4, 1) == 4, "fail");
    static_assert(pow(7, 0) == 1, "fail");
    static_assert(pow(2, 10) == 1024, "fail");
}

void test_hash() {
    static_assert(hash("abacaba", 13, 17239) == 7755, "fail");
    static_assert(hash("", 19, 21) == 0, "fail");
    static_assert(hash("Hello, world!", 257, 1000000007) == 977585366, "fail");
}
