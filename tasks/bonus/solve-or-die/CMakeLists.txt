add_catch(test_solve_or_die statusor.cc status.cc status_payload_printer.cc status_builder.cc test.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-but-set-variable")
