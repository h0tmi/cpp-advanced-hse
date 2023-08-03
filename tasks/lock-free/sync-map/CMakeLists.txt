add_gtest(test_sync_map sync_map_test.cpp hazard_ptr.cpp)
add_benchmark(bench_sync_map sync_map_bench.cpp hazard_ptr.cpp)
