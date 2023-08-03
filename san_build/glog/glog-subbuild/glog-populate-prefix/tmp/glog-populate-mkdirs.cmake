# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-src"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-build"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/tmp"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/src/glog-populate-stamp"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/src"
  "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/src/glog-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/src/glog-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/glog/glog-subbuild/glog-populate-prefix/src/glog-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
