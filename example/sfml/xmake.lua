add_rules("mode.debug", "mode.release")

add_requires("sfml")

set_languages("c++17")

target("sfml")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("sfml")

