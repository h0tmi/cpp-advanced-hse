# Racket Interpreter

## Consuming

With CMake:
```cmake
add_subdirectory(racket)
target_link_directories(my_library PRIVATE racket::racket)
```

## Developing

Build with tests and benchmarks enabled:
```shell
mkdir build && cd build
cmake .. -D RACKET_ENABLE_TESTS=ON -D RACKET_ENABLE_BENCHMARKS=ON
```

Build and run test:
```shell
make test_interpreter
tests/test_interpreter
```

Build and run benchmark:
```shell
make benchmark_interpreter
benchmarks/benchmark_interpreter
```
