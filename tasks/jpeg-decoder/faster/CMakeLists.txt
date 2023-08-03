# You can add your .cpp files at sources.cmake
include(sources.cmake)

link_decoder_deps(decoder_faster)
target_link_libraries(test_decoder_faster decoder_faster)
target_include_directories(test_decoder_faster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
