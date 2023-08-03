set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

message("C++ version: ${CMAKE_CXX_STANDARD}")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
endif()

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
    CACHE STRING "Compiler flags in asan build"
    FORCE)

set(CMAKE_CXX_FLAGS_TSAN "-g -fsanitize=thread -fno-sanitize-recover=all"
  CACHE STRING "Compiler flags in tsan build"
  FORCE)

set(CMAKE_CXX_FLAGS_MSAN "-g -fsanitize=memory -fsanitize-recover=all"
  CACHE STRING "Compiler flags in msan build"
  FORCE)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_ASAN} -fprofile-instr-generate -fcoverage-mapping")
