#----------------------------------------------------------------
# Generated CMake target import file for configuration "ASAN".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glog::glog" for configuration "ASAN"
set_property(TARGET glog::glog APPEND PROPERTY IMPORTED_CONFIGURATIONS ASAN)
set_target_properties(glog::glog PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_ASAN "CXX"
  IMPORTED_LOCATION_ASAN "${_IMPORT_PREFIX}/lib/libglog.a"
  )

list(APPEND _cmake_import_check_targets glog::glog )
list(APPEND _cmake_import_check_files_for_glog::glog "${_IMPORT_PREFIX}/lib/libglog.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
