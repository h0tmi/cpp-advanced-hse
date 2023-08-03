add_gtest(test_executors
  test_executors.cpp
  test_future.cpp
  executors.cpp)

add_benchmark(bench_executors
  run.cpp
  executors.cpp)