set_languages("c++23")
add_requires("glfw")

target("demo-opengl")
    set_kind("binary")
    set_default(false)
    add_files("demo_opengl.cpp")
    add_deps("shimera-opengl")
    add_packages("glfw")