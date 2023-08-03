# You can add your .cpp files at sources.cmake
include(sources.cmake)

link_decoder_deps(decoder_progressive)
target_link_libraries(test_decoder_progressive decoder_progressive)
target_include_directories(test_decoder_progressive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
