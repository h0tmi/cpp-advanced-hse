set(TOKENIZER_TESTS
    tests/test_tokenizer.cpp)

add_catch(test_scheme_tokenizer ${TOKENIZER_TESTS})
    
include(sources.cmake)

target_include_directories(scheme_tokenizer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SCHEME_COMMON_DIR})

target_link_libraries(test_scheme_tokenizer scheme_tokenizer)
