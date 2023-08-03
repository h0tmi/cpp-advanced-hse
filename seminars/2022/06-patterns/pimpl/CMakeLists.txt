add_custom_target(pimpl-all)

add_executable(pimpl-00-no-pimpl 00-no-pimpl/main.cpp 00-no-pimpl/string-splitter.cpp)
add_executable(pimpl-01-raw-ptr 01-raw-ptr/main.cpp 01-raw-ptr/string-splitter.cpp)
add_executable(pimpl-02-unique-ptr 02-unique-ptr/main.cpp 02-unique-ptr/string-splitter.cpp)
add_executable(pimpl-03-fast-pimpl 03-fast-pimpl/main.cpp 03-fast-pimpl/string-splitter.cpp)

add_dependencies(pimpl-all
  pimpl-00-no-pimpl
  pimpl-01-raw-ptr
  pimpl-02-unique-ptr
  pimpl-03-fast-pimpl
)

# You need boost to build this example:
#
# OS X   : brew install boost
# Ubuntu : sudo apt-get install libboost-all-dev

find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
