# Shimera

Shimera is a C++ library designed to allow its users (mainly developers) to implement visual effects on top of their graphical projects without using any GPU knowledge. The library provides a set of shaders that can be easily integrated into OpenGL, SFML, and Raylib projects. Each shader comes with a callable function that allows users to implement and modify the shader's parameters according to their needs.

Our goal is to provide a simple and efficient way for developers to add visual effects to their projects without even touching shader code.

![Example](./res/assets/Distorsion-AberrationChromatic.gif)

## Requirements

- **C++23** or higher
- **OpenGL 3.3+**
- **xmake**

> **Note:** Library dependencies (**GLEW**, **GLM**, and your chosen backend's package such as SFML or Raylib) are declared via `add_requires()` and fetched/built automatically by xmake on first build. You don't need to install them yourself.

## Installation

### Quick Installation

Download the latest release from here: https://github.com/ShimeraTeam/Shimera/releases
> There will be multiple version of the library, choose the one that is compatible with your project's graphics library. (If you have a SFML project on windows x64, choose `shimera-X.X.X-sfml-windows-x64.zip`)

In your xmake.lua, add this to your target:
```
-- add glew and glm as requirements in addition to your library or other dependencies
-- add_requires(..., "glew", "glm")

-- In your target:
add_includedirs("path/to/shimera-0.3.6/include")
add_linkdirs("path/to/shimera-0.3.6/lib")
if is_mode("debug") then
    add_links("shimera-raylib-s-d")
else
    add_links("shimera-raylib-s")
end
```

If you don't have a xmake.lua already or just starting a new project, here is a example:
```
add_rules("mode.debug", "mode.release")

add_requires("raylib", "glew", "glm")

target("my_raylib_shimera") -- rename this if you want
    set_kind("binary")
    set_languages("c++23")
    add_files("src/*.cpp")
    add_packages("raylib", "glew", "glm")

    add_includedirs("path/to/shimera-0.3.6/include")
    add_linkdirs("path/to/shimera-0.3.6/lib")
    if is_mode("debug") then
        add_links("shimera-raylib-s-d")
    else
        add_links("shimera-raylib-s")
    end

```

> You can use other build tools like CMake, Meson and more, but we just don't provide any explanations for them.

## Build Examples

To build the project's examples:
```bash
xmake b opengl-example
xmake b sfml-example
xmake b raylib-example
```

## Project Structure
```
shimera/
├── include/                    # Public headers
│   ├── backend/
│   │   ├── opengl/
│   │   ├── raylib/
│   │   └── sfml/
│   ├── effects/
│   │   └── materials/
│   ├── scene/
│   ├── converts/
│   ├── uniform/
│   └── shimera.h
├── src/                        # Source files
│   ├── backend/
│   │   ├── opengl/
│   │   ├── raylib/
│   │   └── sfml/
│   ├── effects/
│   │   └── materials/
│   ├── scene/
│   └── converts/
├── res/                        # Runtime resources (shaders)
│   └── shader/
│       ├── postprocessing/
│       └── material/
├── examples/
│   ├── opengl/
│   ├── raylib/
│   └── sfml/
├── build/                      # Build artifacts
│   └── linux/x86_64/release/
│       ├── libshimera-sfml.a   # Static library
│       └── libshimera-sfml.so  # Shared library (if built)
├── docs/
├── xmake.lua
└── README.md
```

## Contributing

Shimera is an open-source project, and it needs your help to grow! If you want to contribute, please follow these [contributing guidelines](CONTRIBUTING.md).

## License

Shimera is licensed under the **GNU General Public License v3.0** (`GPL-3.0-only`). See the [LICENSE](LICENSE) file for the full text and [AUTHORS.md](AUTHORS.md) for the copyright holders.

You are free to use, study, share, and modify Shimera, but any distributed work based on it must also remain open source under the GPL-3.0. See <https://www.gnu.org/licenses/gpl-3.0.html> for a summary.
