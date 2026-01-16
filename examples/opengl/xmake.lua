add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_toolchains("msvc")
end
set_languages("cxx17")

add_requires("glfw", "glew")

target("opengl-example")
    set_kind("binary")
    set_rundir("$(projectdir)")
    add_files("../common/*.cpp")
    add_files("src/*.cpp")

    add_linkdirs("../../build/linux/x86_64/release")
    add_links("shimera")
    add_includedirs("../../include")

    add_packages("glfw", "glew")

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end