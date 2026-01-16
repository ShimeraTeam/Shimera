add_rules("mode.debug", "mode.release")

add_requires("sfml", "glew")

set_languages("c++17")

target("sfml-example")
    set_kind("binary")
    add_files("src/*.cpp")

    add_linkdirs("../../build/linux/x86_64/release")
    add_links("shimera")
    add_includedirs("../../include")

    add_packages("sfml", "glew")

