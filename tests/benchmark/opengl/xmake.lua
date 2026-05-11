set_languages("c++23")
add_requires("glfw")

target("bench-opengl")
    set_kind("binary")
    set_default(false)
    add_files("bench_opengl.cpp")
    add_deps("shimera-opengl")
    add_packages("glfw")