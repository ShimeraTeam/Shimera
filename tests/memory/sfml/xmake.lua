set_languages("c++23")

target("test-memory-sfml")
    set_kind("binary")
    set_default(false)
    add_files("test_memory_sfml.cpp")
    add_deps("shimera-sfml")
    if is_plat("linux") then
        add_cxflags("-fsanitize=address,leak", "-g", "-O1")
        add_ldflags("-fsanitize=address,leak")
    end