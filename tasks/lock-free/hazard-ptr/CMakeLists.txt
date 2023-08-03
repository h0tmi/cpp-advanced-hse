add_gtest(test_hazard_ptr 
    hazard_ptr_test.cpp hazard_ptr.cpp)

add_benchmark(bench_hazard_ptr
    hazard_ptr_bench.cpp hazard_ptr.cpp)
