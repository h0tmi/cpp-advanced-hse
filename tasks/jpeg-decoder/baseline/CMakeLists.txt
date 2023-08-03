# You can add your .cpp files at sources.cmake
include(sources.cmake)

link_decoder_deps(decoder_baseline)
target_link_libraries(test_decoder_baseline decoder_baseline)
target_include_directories(test_decoder_baseline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
