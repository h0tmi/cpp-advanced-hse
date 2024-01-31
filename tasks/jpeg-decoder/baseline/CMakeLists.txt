# You can add your .cpp files at sources.cmake
include(sources.cmake)

target_include_directories(decoder_baseline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_decoder_deps(decoder_baseline)
target_link_libraries(test_decoder_baseline decoder_baseline)
