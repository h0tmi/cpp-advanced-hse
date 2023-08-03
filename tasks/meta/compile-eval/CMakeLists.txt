
add_catch(test_templates test_templates.cpp)
set_property(TARGET test_templates PROPERTY CXX_STANDARD 98)
add_catch(test_constexpr_simple test_constexpr_simple.cpp)
set_property(TARGET test_constexpr_simple PROPERTY CXX_STANDARD 11)
add_catch(test_constexpr test_constexpr.cpp)
set_property(TARGET test_constexpr PROPERTY CXX_STANDARD 14)
