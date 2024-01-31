find_package(PNG)
find_package(JPEG)
find_package(FFTW)

# If you have problems with libraries
# (paths below are displayed empty in CMake log or build does not work),
# then you are possibly on MacOS. First, uncomment these lines and retry.
# If this did not help, try to find path to your libs by running
# `brew --prefix fftw` and replacing /usr/local/opt/fftw below with your location.
# (And respectively with libpng and jpeg.)

# set(PNG_INCLUDE_DIRS  /usr/local/opt/libpng/include)
# set(PNG_LIBRARY       /usr/local/opt/libpng/lib/libpng.dylib)
# set(JPEG_INCLUDE_DIRS /usr/local/opt/jpeg/include)
# set(JPEG_LIBRARIES    /usr/local/opt/jpeg/libjpeg.dylib)
# set(FFTW_INCLUDES     /usr/local/opt/fftw/include)
# set(FFTW_LIBRARIES    /usr/local/opt/fftw/lib/libfftw3.dylib)

message(STATUS "Path to PNG includes: ${PNG_INCLUDE_DIRS}")
message(STATUS "Path to PNG library: ${PNG_LIBRARY}")
message(STATUS "Path to JPEG includes: ${JPEG_INCLUDE_DIRS}")
message(STATUS "Path to JPEG library: ${JPEG_LIBRARIES}")
message(STATUS "Path to FFTW includes: ${FFTW_INCLUDES}")
message(STATUS "Path to FFTW library: ${FFTW_LIBRARIES}")

add_catch(test_decoder_huffman 
    huffman/tests/test_huffman.cpp
    utils/logger_init.cpp
)

add_catch(test_decoder_fftw
    fftw/tests/test_fft.cpp
    utils/logger_init.cpp
)

set(DECODER_UTIL_FILES
    utils/logger_init.cpp
    utils/libjpg_reader.cpp
    utils/png_encoder.cpp 
    utils/test_commons.cpp
)

add_catch(test_decoder_baseline
    huffman/tests/test_huffman.cpp
    fftw/tests/test_fft.cpp
    baseline/tests/test_baseline.cpp
    ${DECODER_UTIL_FILES}
)

add_catch(test_decoder_faster
    huffman/tests/test_huffman.cpp
    fftw/tests/test_fft.cpp
    baseline/tests/test_baseline.cpp
    faster/tests/test_faster.cpp
    ${DECODER_UTIL_FILES}
)

if (NOT TEST_SOLUTION)
    add_catch(test_decoder_progressive 
        huffman/tests/test_huffman.cpp
        fftw/tests/test_fft.cpp
        baseline/tests/test_baseline.cpp
        faster/tests/test_faster.cpp
        progressive/tests/test_progressive.cpp
        ${DECODER_UTIL_FILES}
    )
endif ()

target_compile_definitions(test_decoder_baseline PUBLIC HSE_TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_definitions(test_decoder_faster PUBLIC HSE_TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
if (NOT TEST_SOLUTION)
    target_compile_definitions(test_decoder_progressive PUBLIC HSE_TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
endif ()

if (GRADER)
    target_compile_definitions(test_decoder_baseline PUBLIC HSE_ARTIFACTS_DIR="/tmp/artifacts")
    if (NOT TEST_SOLUTION)
        target_compile_definitions(test_decoder_progressive PUBLIC HSE_ARTIFACTS_DIR="/tmp/artifacts")
    endif ()
endif ()

set(DECODER_UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

function(link_decoder_deps TARGET)
    target_include_directories(${TARGET} PUBLIC
            ${PNG_INCLUDE_DIRS}
            ${FFTW_INCLUDES}
            ${JPEG_INCLUDE_DIRS}
            ${DECODER_UTILS_DIR})

    target_link_libraries(${TARGET} PUBLIC
            ${FFTW_LIBRARIES}
            ${PNG_LIBRARY}
            ${JPEG_LIBRARIES}
            glog::glog)

    get_target_property(GLOG_INCLUDES glog::glog INCLUDE_DIRECTORIES)
    target_include_directories(${TARGET} SYSTEM PUBLIC ${GLOG_INCLUDES})

endfunction()

add_subdirectory(huffman)
add_subdirectory(fftw)
add_subdirectory(baseline)
add_subdirectory(faster)

if (NOT TEST_SOLUTION)
    add_subdirectory(progressive)
endif ()

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "^Clang$")
    message(WARNING "Clang is required for fuzzing tests (Apple Clang does not fit too). This is just warning, you can use your current compiler for all tasks except fuzzing. Go to tasks/jpeg-decoder/README.md for guide about clang installation.")
else()
    add_executable(fuzz_decoder_huffman huffman/tests/fuzz_huffman.cpp)
    add_executable(fuzz_decoder_fftw fftw/tests/fuzz_fft.cpp)
    add_executable(fuzz_decoder_baseline baseline/tests/fuzz_jpeg.cpp)
    add_executable(fuzz_decoder_faster baseline/tests/fuzz_jpeg.cpp)
    set_property(TARGET fuzz_decoder_huffman APPEND PROPERTY COMPILE_OPTIONS "-fsanitize=fuzzer-no-link")
    set_property(TARGET fuzz_decoder_fftw APPEND PROPERTY COMPILE_OPTIONS "-fsanitize=fuzzer-no-link")
    set_property(TARGET fuzz_decoder_baseline APPEND PROPERTY COMPILE_OPTIONS "-fsanitize=fuzzer-no-link")
    set_property(TARGET fuzz_decoder_faster APPEND PROPERTY COMPILE_OPTIONS "-fsanitize=fuzzer-no-link")
    target_link_libraries(fuzz_decoder_huffman decoder_huffman "-fsanitize=fuzzer")
    target_link_libraries(fuzz_decoder_fftw decoder_fftw ${FFTW_LIBRARIES} "-fsanitize=fuzzer")
    target_link_libraries(fuzz_decoder_baseline decoder_baseline ${FFTW_LIBRARIES} "-fsanitize=fuzzer")
    target_link_libraries(fuzz_decoder_faster decoder_faster ${FFTW_LIBRARIES} "-fsanitize=fuzzer")
endif ()
