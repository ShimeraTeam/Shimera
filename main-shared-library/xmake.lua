add_rules("mode.debug", "mode.release")

add_requires("glew")
add_requires("glfw")

set_languages("c++17")

target("main-lib")
    set_kind("shared")
    add_files("src/*.cpp")
    add_files("src/vendor/stb_image/stb_image.cpp")

    add_includedirs("include", {public = true})
    add_includedirs("src/vendor/stb_image")
    add_packages("glew")
    add_packages("glfw")