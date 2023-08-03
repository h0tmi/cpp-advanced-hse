#include <cstddef>

template <size_t N>
static constexpr size_t kCountDistinctPrimeDivisors = [] {
    size_t cnt = 0;
    size_t num = N;

    for (size_t i = 2; i * i <= N; ++i) {
        bool has = false;

        while (num % i == 0) {
            num /= i;
            has = true;
        }

        cnt += has;
    }

    return cnt;
}();  // IILE

auto main() -> int {
    static_assert(kCountDistinctPrimeDivisors<100> == 2);
    static_assert(kCountDistinctPrimeDivisors<3 * 5 * 7 * 11> == 4);
}
