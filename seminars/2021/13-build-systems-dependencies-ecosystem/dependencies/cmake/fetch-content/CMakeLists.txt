cmake_minimum_required(VERSION 3.18)

project(fetch-content-deps)

set(CMAKE_CXX_STANDARD 20)

# `FetchContent` is a CMake module -- it needs to be imported via `include`.
include(FetchContent)

# Fetch `MPark.Patterns` library.
FetchContent_Declare(
  mpark_patterns
  GIT_REPOSITORY https://github.com/mpark/patterns.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(mpark_patterns)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE mpark_patterns)
