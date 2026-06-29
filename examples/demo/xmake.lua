add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_toolchains("msvc")
end

add_requires("raylib", "glew")

set_languages("c++23")

target("demo")
    set_kind("binary")
    set_default(false)
    set_group("examples")
    set_targetdir("$(builddir)/$(plat)/$(arch)/$(mode)")
    -- only the active main is built; other mainX.cpp variants are kept aside.
    -- swap one in by renaming it to src/main.cpp, then just build.
    add_files("src/main.cpp")

    -- `mv` preserves a file's old mtime, so after renaming a variant onto
    -- main.cpp xmake would think the cached object is still up to date and
    -- skip it. Touching the file before every build forces a fresh compile.
    before_build(function (target)
        local main = path.join(os.scriptdir(), "src/main.cpp")
        os.touch(main, {mtime = os.time()})
    end)

    add_deps("shimera-raylib")
    add_includedirs("../../include")

    add_packages("raylib", "glew")

    if is_plat("windows") then
        add_syslinks("opengl32")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL")
    else
        add_syslinks("GL")
    end
