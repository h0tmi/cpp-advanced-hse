add_gtest(test_isprime test.cpp is_prime.cpp)

add_benchmark(bench_isprime run.cpp is_prime.cpp)
