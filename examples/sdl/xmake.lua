add_rules("mode.debug", "mode.release")

add_requires("libsdl3", "glew")

set_languages("c++17")

target("sdl-example")
    set_kind("binary")
    set_default(false)
    set_group("examples")
    set_targetdir("$(builddir)/$(plat)/$(arch)/$(mode)")
    add_files("src/*.cpp")

    add_deps("shimera-sdl")
    add_includedirs("../../include")

    add_packages("libsdl3", "glew")

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end

