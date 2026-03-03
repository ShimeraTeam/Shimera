add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_toolchains("msvc")
end

add_requires("glew")
add_requires("sfml", {optional = true})
add_requires("libsdl3", {optional = true})
add_requires("raylib", {optional = true})

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

target("shimera-opengl")
    set_kind("$(kind)")
    set_default(true)
    add_files("src/*.cpp")
    add_files("src/backend/*.cpp")
    add_headerfiles("include/(*.h)")
    add_headerfiles("include/(*.hpp)")
    add_headerfiles("include/uniform/(*.hpp)")
    add_includedirs("include", {public = true})
    add_packages("glew", {public = true})
    add_defines("SHIMERA_BACKEND_OPENGL", {public = true})

    if has_config("shared") then
        set_kind("shared")
        add_defines("SHIMERA_EXPORTS")
        add_defines("SHIMERA_SHARED", {public = true})
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

target("shimera-sfml")
    set_kind("$(kind)")
    set_default(false)
    add_files("src/*.cpp")
    add_files("src/backend/*.cpp")
    add_files("src/backend/sfml/*.cpp")
    add_files("src/effects/*.cpp")
    add_headerfiles("include/(*.h)")
    add_headerfiles("include/(*.hpp)")
    add_headerfiles("include/uniform/(*.hpp)")
    add_headerfiles("include/backend/sfml/(*.hpp)")
    add_headerfiles("include/effects/(*.hpp)")
    add_includedirs("include", {public = true})
    add_packages("glew", {public = true})
    add_packages("sfml", {public = true})
    add_defines("SHIMERA_BACKEND_SFML", {public = true})

    if has_config("shared") then
        set_kind("shared")
        add_defines("SHIMERA_EXPORTS")
        add_defines("SHIMERA_SHARED", {public = true})
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

target("shimera-sdl")
    set_kind("$(kind)")
    set_default(false)
    add_files("src/*.cpp")
    add_files("src/backend/*.cpp")
    add_headerfiles("include/(*.h)")
    add_headerfiles("include/(*.hpp)")
    add_headerfiles("include/uniform/(*.hpp)")
    add_includedirs("include", {public = true})
    add_packages("glew", {public = true})
    add_packages("libsdl3", {public = true})
    add_defines("SHIMERA_BACKEND_SDL", {public = true})

    if has_config("shared") then
        set_kind("shared")
        add_defines("SHIMERA_EXPORTS")
        add_defines("SHIMERA_SHARED", {public = true})
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

target("shimera-raylib")
    set_kind("$(kind)")
    set_default(false)
    add_files("src/*.cpp")
    add_files("src/backend/*.cpp")
    add_files("src/backend/raylib/*.cpp")
    add_files("src/effects/*.cpp")
    add_headerfiles("include/(*.h)")
    add_headerfiles("include/(*.hpp)")
    add_headerfiles("include/uniform/(*.hpp)")
    add_headerfiles("include/backend/raylib/(*.hpp)")
    add_headerfiles("include/effects/(*.hpp)")
    add_includedirs("include", {public = true})
    add_packages("glew", {public = true})
    add_packages("raylib", {public = true})
    add_defines("SHIMERA_BACKEND_RAYLIB", {public = true})

    if has_config("shared") then
        set_kind("shared")
        add_defines("SHIMERA_EXPORTS")
        add_defines("SHIMERA_SHARED", {public = true})
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
    includes("examples/raylib/xmake.lua")
end
