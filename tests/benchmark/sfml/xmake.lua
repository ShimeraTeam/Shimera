set_languages("c++23")
add_requires("nlohmann_json")

target("sfml-tests")
    set_kind("binary")
    set_default(false)
    add_files("*.cpp")
    add_files("../../*.cpp")
    add_files("../../memory/*.cpp")
    add_headerfiles("*.hpp")
    add_headerfiles("../../*.hpp")
    add_headerfiles("../../memory/*.cpp")
    add_deps("shimera-sfml")
    add_packages("nlohmann_json")
    if is_plat("linux") then
        add_cxflags("-fsanitize=address,leak", "-g", "-O1")
        add_ldflags("-fsanitize=address,leak")
    end