cmake_minimum_required(VERSION 3.18)

project(find-package-deps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.7
  REQUIRED
  COMPONENTS system thread
)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${Boost_LIBRARIES})
