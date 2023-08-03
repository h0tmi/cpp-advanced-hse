cmake_minimum_required(VERSION 3.13)

project(pimpl)

set(CMAKE_CXX_STANDARD 20)

add_executable(no-pimpl no-pimpl/main.cpp no-pimpl/string-splitter.cpp)
add_executable(raw-ptr raw-ptr/main.cpp raw-ptr/string-splitter.cpp)
add_executable(unique-ptr unique-ptr/main.cpp unique-ptr/string-splitter.cpp)
add_executable(fast fast/main.cpp fast/string-splitter.cpp)

# OS X   : brew install boost
# Ubuntu : sudo apt-get install libboost-all-dev

find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
