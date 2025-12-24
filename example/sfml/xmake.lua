add_rules("mode.debug", "mode.release")

add_requires("sfml", "glew")

set_languages("c++17")

target("sfml")
    set_kind("binary")
    add_files("src/*.cpp")

    add_linkdirs("../common/build/linux/x86_64/release")
    add_links("shimera")
    add_includedirs("../common")

    add_packages("sfml", "glew")

