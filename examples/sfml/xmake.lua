add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_toolchains("msvc")
end

add_requires("sfml", "glew")

set_languages("c++17")

target("sfml-example")
    set_kind("binary")
    set_default(false)
    set_group("examples")
    set_targetdir("$(builddir)/$(plat)/$(arch)/$(mode)")
    add_files("src/*.cpp")

    add_deps("shimera-sfml")
    add_includedirs("../../include")

    add_packages("sfml", "glew")

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end

