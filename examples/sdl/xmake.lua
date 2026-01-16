add_rules("mode.debug", "mode.release")

add_requires("libsdl3", "glew")

set_languages("c++17")

target("sdl-example")
    set_kind("binary")
    add_files("src/*.cpp")

    add_linkdirs("../../build/linux/x86_64/release")
    add_links("shimera")
    add_includedirs("../../include")

    add_packages("libsdl3", "glew")

