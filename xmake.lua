add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_toolchains("msvc")
end

add_requires("glew")

set_languages("c++17")

option("shared")
    set_default(false)
    set_showmenu(true)
    set_description("Build shared library instead of static")
option_end()

option("examples")
    set_default(true)
    set_showmenu(true)
    set_description("Build example projects")
option_end()

target("shimera")
    set_kind("$(kind)")
    set_default(true)
    add_files("src/*.cpp")
    add_headerfiles("include/(*.h)")
    add_headerfiles("include/(*.hpp)")
    add_headerfiles("include/uniform/(*.hpp)")
    add_includedirs("include", {public = true})
    add_packages("glew", {public = true})

    if has_config("shared") then
        set_kind("shared")
        add_defines("SHIMERA_EXPORTS")
    else
        set_kind("static")
    end

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end

if has_config("examples") then
    includes("examples/opengl/xmake.lua")
    includes("examples/sdl/xmake.lua")
    includes("examples/sfml/xmake.lua")
end

