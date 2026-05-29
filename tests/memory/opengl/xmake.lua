set_languages("c++23")
add_requires("glfw")

target("test-memory-opengl")
    set_kind("binary")
    set_default(false)
    add_files("test_memory_opengl.cpp")
    add_deps("shimera-opengl")
    add_packages("glfw")
    if is_plat("linux") then
        add_cxflags("-fsanitize=address,leak", "-g", "-O1")
        add_ldflags("-fsanitize=address,leak")
    end