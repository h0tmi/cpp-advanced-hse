set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/glog)
set(WITH_GFLAGS off)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

FetchContent_Declare(
        Glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG        ee6faf13b20de9536f456bd84584f4ab4db1ceb4
        CMAKE_ARGS -DBUILD_TESTING=OFF
)

FetchContent_MakeAvailable(Glog)
