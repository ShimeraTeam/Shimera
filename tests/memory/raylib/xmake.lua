set_languages("c++23")

target("test-memory-raylib")
    set_kind("binary")
    set_default(false)
    add_files("test_memory_raylib.cpp")
    add_deps("shimera-raylib")
    if is_plat("linux") then
        add_cxflags("-fsanitize=address,leak", "-g", "-O1")
        add_ldflags("-fsanitize=address,leak")
    end