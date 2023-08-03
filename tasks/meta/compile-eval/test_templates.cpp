#include <pow.h>
#include <sqrt.h>

#define STATIC_ASSERT(condition) do {\
    char x[condition ? 1 : -1] __attribute__((unused));\
} while (0)

void test_pow() {
    STATIC_ASSERT((Pow<2, 2>::value == 4));
    STATIC_ASSERT((Pow<3, 4>::value == 81));
    STATIC_ASSERT((Pow<1, 10>::value == 1));
    STATIC_ASSERT((Pow<4, 1>::value == 4));
    STATIC_ASSERT((Pow<7, 0>::value == 1));
    STATIC_ASSERT((Pow<2, 10>::value == 1024));
}

void test_sqrt() {
    STATIC_ASSERT((Sqrt<9>::value == 3));
    STATIC_ASSERT((Sqrt<1>::value == 1));
    STATIC_ASSERT((Sqrt<2>::value == 2));
    STATIC_ASSERT((Sqrt<10>::value == 4));
    STATIC_ASSERT((Sqrt<143>::value == 12));
    STATIC_ASSERT((Sqrt<100000000>::value == 10000));
    STATIC_ASSERT((Sqrt<1000000007>::value == 31623));
}
