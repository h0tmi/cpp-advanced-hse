# Install script for directory: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/bonus

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "ASAN")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/bad-hash/cmake_install.cmake")
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/bad-rand/cmake_install.cmake")
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/solve-or-die/cmake_install.cmake")
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/matrix-2.0/cmake_install.cmake")
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/executors/cmake_install.cmake")
  include("/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/bonus/clang-fun/cmake_install.cmake")

endif()

