add_rules("mode.debug", "mode.release")

add_requires("libsdl3")

set_languages("c++17")

target("libsdl3")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("libsdl3")

