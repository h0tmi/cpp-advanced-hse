cmake_minimum_required(VERSION 3.18)

project(git-deps)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(dependencies)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE spdlog::spdlog)
