add_rules("mode.debug", "mode.release")

add_requires("raylib", "glew")

set_languages("c++23")

target("raylib-example")
    set_kind("binary")
    add_files("src/*.cpp")

    add_packages("raylib", "glew")