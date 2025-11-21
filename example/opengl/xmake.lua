add_rules("mode.debug", "mode.release")

-- install dependencies in the local folder instead of the global path
--set_policy("package.install_locally", true)

if is_plat("windows") then
    set_toolchains("msvc")
end
set_languages("cxx17")

add_requires("glfw", "glew")

target("opengl_learning")
    set_kind("binary")
    set_rundir("$(projectdir)")
    add_files("src/*.cpp")
    add_packages("glfw", "glew")

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end