add_rules("mode.debug", "mode.release")

add_requires("glew")

set_languages("c++17")

-- Librairie statique
target("shimera")
    set_kind("static")
    add_files("*.cpp")
    add_headerfiles("*.h", "*.hpp")
    add_headerfiles("uniform/*.hpp")
    add_headerfiles("shimera.h")
    add_includedirs(".", {public = true})
    add_packages("glew", {public = true})

-- Librairie shared
target("shimera-shared")
    set_kind("shared")
    set_basename("shimera")
    add_files("*.cpp")
    add_headerfiles("*.h", "*.hpp")
    add_headerfiles("uniform/*.hpp")
    add_headerfiles("shimera.h")
    add_includedirs(".", {public = true})
    add_packages("glew", {public = true})
    add_defines("SHIMERA_EXPORTS")
