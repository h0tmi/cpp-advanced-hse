cmake_minimum_required(VERSION 3.8)
project(coroutine)

if (TEST_SOLUTION)
  include_directories(../private/coroutine)
endif()

find_package(Boost
    1.64
    REQUIRED
    COMPONENTS context)

include_directories(${Boost_INCLUDE_DIRS})

add_gtest(test_coroutine test.cpp)
target_link_libraries(test_coroutine ${Boost_LIBRARIES})
