option(TEST_SOLUTION "Build solution" OFF)
option(GRADER "Building on CI" OFF)

function(add_hse_executable NAME)
    add_executable(${NAME} ${ARGN})
    target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(add_hse_test_binary TARGET)
    add_hse_executable(${TARGET} ${ARGN})

    if (TEST_SOLUTION)
        add_custom_target(
                run_${TARGET}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${TARGET}
                COMMAND ${CMAKE_BINARY_DIR}/${TARGET})

        add_dependencies(test-all run_${TARGET})
    endif ()
endfunction()

function(add_catch TARGET)
    add_hse_executable(${TARGET} ${ARGN})
    target_link_libraries(${TARGET} contrib_catch_main)

    if (TEST_SOLUTION)
        add_custom_target(
                run_${TARGET}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${TARGET}
                COMMAND ${CMAKE_BINARY_DIR}/${TARGET})

        add_dependencies(test-all run_${TARGET})
    endif ()
endfunction()

function(add_gtest TARGET)
    add_hse_test_binary(${TARGET}
            ${CMAKE_SOURCE_DIR}/contrib/gmock_main.cc
            ${ARGN})

    target_link_libraries(${TARGET} gmock)
endfunction()

function(add_benchmark TARGET)
    add_hse_test_binary(${TARGET} ${ARGN})

    target_link_libraries(${TARGET} benchmark)
endfunction()

add_custom_target(test-all)
add_custom_target(build-ctf)
add_dependencies(test-all build-ctf) # Build ctf tasks in CI

function(add_ctf)
    cmake_parse_arguments(PARSED_ARGS "" "NAME" "SOURCES;OPTIONS" ${ARGN})
    set(TARGET ctf_${PARSED_ARGS_NAME})
    add_executable(${TARGET} ${PARSED_ARGS_SOURCES})
    target_compile_options(${TARGET} PRIVATE ${PARSED_ARGS_OPTIONS})
    target_link_options(${TARGET} PRIVATE ${PARSED_ARGS_OPTIONS})
    add_dependencies(build-ctf ${TARGET})
endfunction()
