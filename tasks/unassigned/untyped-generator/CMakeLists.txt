cmake_minimum_required(VERSION 3.8)
project(untyped-generator)

if (TEST_SOLUTION)
  include_directories(../private/untyped-generator)
endif()

find_package(Boost
    1.64
    REQUIRED
    COMPONENTS context)

include_directories(${Boost_INCLUDE_DIRS})

add_gtest(test_untyped_generator test.cpp)
target_link_libraries(test_untyped_generator ${Boost_LIBRARIES})
