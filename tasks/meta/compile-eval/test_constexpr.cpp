#include <next_prime.h>
#include <determinant.h>

void test_next_prime() {
    static_assert(next_prime(1) == 2, "fail");
    static_assert(next_prime(2) == 2, "fail");
    static_assert(next_prime(9) == 11, "fail");
    static_assert(next_prime(17230) == 17231, "fail");
    static_assert(next_prime(121) == 127, "fail");
    static_assert(next_prime(1000000) == 1000003, "fail");
    static_assert(next_prime(1000000000) == 1000000007, "fail");
}

void test_determinant() {
    static_assert(determinant<2>({{{2, 3}, {1, 4}}}) == 5, "fail");
    static_assert(determinant<3>({{{1,2,3},{4,5,6},{7,8,9}}}) == 0, "fail");
    static_assert(determinant<1>({{{3}}}) == 3, "fail");
    static_assert(determinant<5>({{
        {3, -5, 1, -4, 2},
        {-5, -4, -2, -3, -2},
        {5, 3, 0, -3, -3},
        {2, -2, 0, 2, -1},
        {-1, -1, 5, -3, -5}
    }}) == -7893, "fail");
}
