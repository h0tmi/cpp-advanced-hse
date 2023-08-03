# Require CMake >= 3.18.
cmake_minimum_required(VERSION 3.18)

# Project name.
project(racket)

# Public options for users to control.
option(RACKET_ENABLE_TESTS "Enable testing of the `racket` library" OFF)
option(RACKET_ENABLE_BENCHMARKS "Enable benchmarking of the `racket` library" OFF)

# Import CMake modules.
include(cmake/Ccache.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Helpers.cmake)
include(cmake/ProjectSettings.cmake)

# Create library target.
add_library(racket
  src/interpreter.cpp
)
set_compile_options(racket)

# Namespace library for users.
add_library(racket::racket ALIAS racket)

# Set search paths.
target_include_directories(racket PUBLIC include)

# Build tests if needed.
if (${RACKET_ENABLE_TESTS})
  add_subdirectory(tests)
endif()

# Build benchmarks if needed.
if (${RACKET_ENABLE_BENCHMARKS})
  add_subdirectory(benchmarks)
endif()
