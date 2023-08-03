cmake_minimum_required(VERSION 3.18)

project(conan-deps)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${CONAN_LIBS})
