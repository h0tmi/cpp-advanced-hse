if (APPLE)
    message(WARNING "Task bonus/clang-fun works only on Linux")
else()
    find_package(LLVM)

    if (${LLVM_FOUND})
        message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
        message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
        message(STATUS "LLVM include dir is ${LLVM_INCLUDE_DIR}")

        if (${LLVM_PACKAGE_VERSION} VERSION_LESS 14)
            message(WARNING "You need at least LLVM 14 to correctly build bonus/clang-fun")
        else()
            add_executable(check-names check_names.cpp)

            target_include_directories(check-names PUBLIC ${LLVM_INCLUDE_DIRS})
            target_compile_definitions(check-names PUBLIC ${LLVM_DEFINITIONS})
            target_link_directories(check-names PUBLIC ${LLVM_LIBRARY_DIRS})
            set_target_properties(check-names PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter -Wno-unused-variable -fno-rtti")

            set(clang_libs  clangFrontend clangSerialization clangDriver clangParse clangASTMatchers
                            clangSema clangCodeGen clangAnalysis clangRewrite
                            clangEdit clangAST clangLex clangTooling clangBasic)
            target_link_libraries(check-names LLVM ${clang_libs})

            set(TESTS_LIST
                tests/no-dict/fun.cpp
                tests/no-dict/perm.cpp
                tests/no-dict/segment.cpp
                tests/dict/test_file.cpp
                tests/dict/sorting.cpp
            )

            foreach (test ${TESTS_LIST})
                get_filename_component(result ${test} NAME_WE)
                add_executable("clang_test_${result}" ${test})
            endforeach(test)
        endif()
    else()
        message(WARNING "LLVM was NOT found, bonus/clang-fun will not build")
    endif()
endif()