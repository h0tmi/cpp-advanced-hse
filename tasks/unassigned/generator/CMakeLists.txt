cmake_minimum_required(VERSION 3.8)
project(generator)

if (TEST_SOLUTION)
  include_directories(../private/generator)
endif()

find_package(Boost
    1.64
    REQUIRED
    COMPONENTS context)

include_directories(${Boost_INCLUDE_DIRS})

add_gtest(test_generator test.cpp)
target_link_libraries(test_generator ${Boost_LIBRARIES})
