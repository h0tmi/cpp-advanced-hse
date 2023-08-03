add_library(benchmark
  contrib/benchmark/src/benchmark_api_internal.cc
  contrib/benchmark/src/benchmark.cc
  contrib/benchmark/src/benchmark_main.cc
  contrib/benchmark/src/benchmark_name.cc
  contrib/benchmark/src/benchmark_register.cc
  contrib/benchmark/src/benchmark_runner.cc
  contrib/benchmark/src/colorprint.cc
  contrib/benchmark/src/commandlineflags.cc
  contrib/benchmark/src/complexity.cc
  contrib/benchmark/src/console_reporter.cc
  contrib/benchmark/src/counter.cc
  contrib/benchmark/src/csv_reporter.cc
  contrib/benchmark/src/json_reporter.cc
  contrib/benchmark/src/reporter.cc
  contrib/benchmark/src/sleep.cc
  contrib/benchmark/src/statistics.cc
  contrib/benchmark/src/string_util.cc
  contrib/benchmark/src/sysinfo.cc
  contrib/benchmark/src/timers.cc)

target_include_directories(benchmark
  PRIVATE contrib/benchmark/src)

target_compile_definitions(benchmark
  PRIVATE HAVE_POSIX_REGEX)

target_include_directories(benchmark
  SYSTEM PUBLIC contrib/benchmark/include)

target_link_libraries(benchmark
  pthread
  dl)
