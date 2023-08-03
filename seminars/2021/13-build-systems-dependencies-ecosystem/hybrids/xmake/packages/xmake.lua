set_languages("c++20")

add_requires("poco")

target("json-parser")
    set_kind("binary")
    add_files("main.cpp")
    add_packages("poco")

