set_languages("c++20")

target("main")
    set_kind("binary")
    add_files("src/*.cpp", "src/*.mpp")
