# You can add your .cpp files at sources.cmake
include(sources.cmake)

link_decoder_deps(decoder_fftw)
target_link_libraries(test_decoder_fftw decoder_fftw)
