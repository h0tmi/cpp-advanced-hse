cmake_minimum_required(VERSION 3.13)

project(cpp-advanced-hse)

option(ENABLE_SEMINARS OFF)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)
include(cmake/SeminarUtils.cmake)

include(FetchContent)
include(cmake/FindGlog.cmake)

find_package(Catch REQUIRED)
find_package(Benchmark REQUIRED)
find_package(GTest REQUIRED)

include_directories(util)

add_subdirectory(allocations_checker)

add_subdirectory(tasks)

# Seminars
if (${ENABLE_SEMINARS})
  set(ALL_SEMINARS_TARGET_NAME "seminars-examples-all")
  add_custom_target(${ALL_SEMINARS_TARGET_NAME})
  add_subdirectory(seminars)
endif()
