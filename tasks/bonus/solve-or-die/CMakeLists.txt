add_catch(test_solve_or_die statusor.cc status.cc status_payload_printer.cc status_builder.cc test.cpp)
target_compile_options(test_solve_or_die PRIVATE -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-but-set-variable)
