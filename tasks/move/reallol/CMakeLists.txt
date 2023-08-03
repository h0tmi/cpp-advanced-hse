if (NOT CMAKE_BUILD_TYPE MATCHES TSAN)
    add_ctf(NAME reallol SOURCES main.cpp OPTIONS -fsanitize=address)
endif ()
