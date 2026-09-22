set_project("shimera-poc")
set_version("0.1.0")
set_xmakever("2.9.0")

add_rules("mode.debug", "mode.release")

set_languages("cxx20")
set_warnings("all")
set_encodings("utf-8")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
end

-- we have the choice to not use certains host libraries: xmake f --sfml=n --raylib=n
option("sfml",   {default = true,  description = "Build the SFML host shim and example"})
option("raylib", {default = true,  description = "Build the raylib host shim and example"})
option("sdl",    {default = true,  description = "Build SDL3 target and example"})
option("slangc", {default = "",    description = "Path to slangc, when it is not in PATH", type = "string"})

add_requires("glew 2.2.0", "glm 1.0.1", "glfw 3.4")
-- Build-time only, converts Slang's Vulkan-flavoured SPIR-V into desktop GLSL 330.
add_requires("spirv-cross 1.3.268+0")

if has_config("sfml") then
    -- graphics + window only. the libs used by Shimera never plays a sound or opens a socket.
    add_requires("sfml 3.0.1", {configs = {audio = false, network = false}})
end
if has_config("raylib") then
    add_requires("raylib 5.5")
end
if has_config("sdl") then
    -- SDL3 core only decodes BMP, the examples' PNG assets need SDL_image.
    add_requires("libsdl3", "libsdl3_image")
end

-- Slang -> SPIR-V -> desktop GLSL 330, run as part of the build.
target("shaders")
    set_kind("phony")
    add_packages("spirv-cross")
    on_build(function (target)
        import("core.project.depend")
        import("lib.detect.find_program")

        -- Search slangc in PATH.
        local slangc = get_config("slangc")
        if slangc == "" then
            slangc = find_program("slangc", {check = "-v"})
        end

        if not slangc then
            cprint("${yellow}warning: slangc not found, skipping Slang shaders " ..
                   "(install Slang or pass --slangc=<path>)")
            return
        end

        local outputdir = path.join(os.projectdir(), "res", "shader", "generated")
        local spvdir = path.join(target:autogendir(), "spv")
        os.mkdir(outputdir)
        os.mkdir(spvdir)

        -- spirv-cross ships with the package rather than being installed system-wide.
        local spirvcross = nil
        for _, pkg in ipairs(target:orderpkgs()) do
            if pkg:name() == "spirv-cross" then
                for _, exe in ipairs({"spirv-cross.exe", "spirv-cross"}) do
                    local candidate = path.join(pkg:installdir(), "bin", exe)
                    if os.isfile(candidate) then
                        spirvcross = candidate
                        break
                    end
                end
            end
        end
        if not spirvcross then
            cprint("${yellow}warning: spirv-cross not found, skipping Slang shaders")
            return
        end

        local slangdir = path.join(os.projectdir(), "res", "shader", "postprocessing")
        local sharedvertex = path.join(slangdir, "postprocess.slang")

        -- slangc => SPIR-V -> spirv-cross => GLSL 330.
        local function compile(sourcefile, entry, stage, ext, io_dir)
            local spv = path.join(spvdir, path.basename(sourcefile) .. "." .. ext .. ".spv")
            local glsl = path.join(outputdir, path.basename(sourcefile) .. "." .. ext)

            -- -no-mangle keeps the original uniform names, which is what lets GLShader find
            -- them through OpenGL's own uses instead of a generated layout table.
            os.vrunv(slangc, {sourcefile, "-entry", entry, "-stage", stage,
                              "-target", "spirv", "-no-mangle", "-o", spv})
            os.vrunv(spirvcross, {spv, "--version", "330", "--no-es", "--no-420pack-extension",
                                  "--rename-interface-variable", io_dir, "0", "v_texCoords",
                                  "--output", glsl})
        end

        -- The shared vertex half, compiled once for every effect (see postprocess.slang).
        if os.isfile(sharedvertex) then
            depend.on_changed(function ()
                cprint("${color.build.object}[slang]${clear} %s (shared vertex)", path.filename(sharedvertex))
                compile(sharedvertex, "vertMain", "vertex", "vert", "out")
            end, {files = {sharedvertex},
                  dependfile = path.join(target:autogendir(), "postprocess.slang.d")})
        end

        -- Every other .slang is an effect, and is only a fragment shader.
        for _, sourcefile in ipairs(os.files(path.join(slangdir, "*.slang"))) do
            if sourcefile ~= sharedvertex then
                local name = path.basename(sourcefile)
                depend.on_changed(function ()
                    cprint("${color.build.object}[slang]${clear} %s", path.filename(sourcefile))
                    compile(sourcefile, "fragMain", "fragment", "frag", "in")
                end, {files = {sourcefile, sharedvertex},
                      dependfile = path.join(target:autogendir(), name .. ".slang.d")})
            end
        end
    end)

-- core, opengl have no idea what host are used
target("shimera")
    set_kind("static")
    add_deps("shaders")

    -- src/hosts is excluded on purpose: host libs are separate targets.
    add_files("src/**.cpp|hosts/**.cpp")
    add_headerfiles("src/(**.h)", "src/(**.hpp)", "src/(**.inl)")

    add_includedirs("src", {public = true})
    add_packages("glew", "glm", {public = true})
    add_defines("SHIMERA_STATIC", {public = true})

    -- POC only: shaders are read from the source tree instead of being installed
    -- or embedded. will eventually test to embed and load shader from memory.
    add_defines("SHIMERA_RES_DIR=R\"($(projectdir)/res)\"", {public = true})

-- host targets
-- note this one links no GLFW: a pure-GL host only needs a current context.
target("shimera-glfw")
    set_kind("static")
    add_files("src/hosts/GlfwTarget.cpp")
    add_deps("shimera")

if has_config("sfml") then
target("shimera-sfml")
    set_kind("static")
    add_files("src/hosts/SfmlTarget.cpp")
    add_deps("shimera")
    add_packages("sfml", {public = true})
end

if has_config("raylib") then
target("shimera-raylib")
    set_kind("static")
    add_files("src/hosts/RaylibTarget.cpp")
    add_deps("shimera")
    add_packages("raylib", {public = true})
end

if has_config("sdl") then
target("shimera-sdl")
    set_kind("static")
    add_files("src/hosts/SdlTarget.cpp")
    add_deps("shimera")
    add_packages("libsdl3", {public = true})
end

-- examples
target("glfw_example")
    set_kind("binary")
    set_default(true)
    add_files("examples/glfw_main.cpp", "examples/Scene.cpp")
    add_deps("shimera-glfw")
    add_packages("glfw")
    set_rundir("$(projectdir)")

if has_config("sfml") then
target("sfml_example")
    set_kind("binary")
    add_files("examples/sfml_main.cpp")
    add_deps("shimera-sfml")
    set_rundir("$(projectdir)")
end

if has_config("raylib") then
target("raylib_example")
    set_kind("binary")
    add_files("examples/raylib_main.cpp")
    add_deps("shimera-raylib")
    set_rundir("$(projectdir)")

-- test material depth-sort
target("raylib3d_example")
    set_kind("binary")
    add_files("examples/raylib3d_main.cpp")
    add_deps("shimera-raylib")
    set_rundir("$(projectdir)")
end

if has_config("sdl") then
-- Plain SDL3 2D example, no Shimera yet: it links the packages directly.
target("sdl_example")
    set_kind("binary")
    add_files("examples/SdlMain.cpp")
    add_deps("shimera-sdl")
    add_packages("libsdl3_image")
    set_rundir("$(projectdir)")
end
