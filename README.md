# Shimera

Shimera is a C++ library designed to allow its users (mainly developers) to implement visual effects on top of their graphical projects without using any GPU knowledge. The library provides a set of shaders that can be easily integrated into OpenGL, SFML, and Raylib projects. Each shader comes with a callable function that allows users to implement and modify the shader's parameters according to their needs.

Our goal is to provide a simple and efficient way for developers to add visual effects to their projects without even touching shader code.

![Example](./res/assets/Distorsion-AberrationChromatic.gif)

## Requirements

- **C++23** or higher
- **OpenGL 3.3+**
- **GLEW** (OpenGL Extension Wrangler Library)
- **xmake**

## Installation

### Quick Installation
```bash
# Clone the repository
git clone https://github.com/yourusername/shimera.git
cd shimera

# Build static library for your backend (static by default)
xmake b shimera-sfml       # SFML backend
xmake b shimera-opengl     # Pure OpenGL backend
xmake b shimera-sdl        # SDL3 backend
xmake b shimera-raylib     # Raylib backend

# Build shared library
xmake f --shared=y
xmake b shimera-sfml       # or any other backend

# Build in release mode
xmake f -m release
xmake b shimera-sfml       # or any other backend

# Install to system (optional)
sudo xmake install shimera-sfml       # or any other backend
```

> **Note:** Shimera is split into backend-specific targets. You must build the target that matches the windowing library you use in your project.

## Using Shimera in Your Project

### Option 1: System Installation

After building, install the library system-wide:
```bash
sudo xmake install shimera-sfml       # or any other backend
```

Then in your project's `xmake.lua`:
```lua
add_rules("mode.debug", "mode.release")
set_languages("c++23")

add_requires("glew")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    add_includedirs("/usr/local/include")
    add_linkdirs("/usr/local/lib")
    add_links("shimera-sfml")
```

### Option 2: Local Library

Build shimera first, then copy the built library files to your project:

#### Using Static Library (.a / .lib)
```bash
xmake b shimera-sfml
cp -r shimera/include /path/to/your/project/
cp shimera/build/linux/x86_64/release/libshimera-sfml.a /path/to/your/project/lib/
```

In your `xmake.lua`:
```lua
add_rules("mode.debug", "mode.release")
set_languages("c++23")

add_requires("glew")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    add_includedirs("include")
    add_linkdirs("lib")
    add_links("shimera-sfml")
```

#### Using Shared Library (.so / .dll)
```bash
xmake f --shared=y
xmake b shimera-sfml
cp -r shimera/include /path/to/your/project/
cp shimera/build/linux/x86_64/release/libshimera-sfml.so /path/to/your/project/lib/
```

In your `xmake.lua`:
```lua
add_rules("mode.debug", "mode.release")
set_languages("c++23")

add_requires("glew")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    add_includedirs("include")
    add_linkdirs("lib")
    add_links("shimera-sfml")
    add_rpathdirs("lib")  -- ensures the .so is found at runtime
```

> **Note:** With shared libraries, you need to ensure libshimera.so is accessible at runtime: 
> - Option 1: Add `add_rpathdirs()` in xmake (recommended) 
> - Option 2: Copy `libshimera.so` next to your executable 
> - Option 3: Add the library path to `LD_LIBRARY_PATH`.

### Option 3: Git Submodule (Recommended for Development)
```bash
cd your-project
git submodule add https://github.com/ShimeraTeam/Shimera.git libs/shimera
```

In your `xmake.lua`:
```lua
add_rules("mode.debug", "mode.release")
set_languages("c++23")

add_requires("glfw", "glew")  -- add your windowing library here

includes("libs/shimera")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")

    add_deps("shimera-sfml")  -- or shimera-opengl, shimera-sdl, shimera-raylib
    add_packages("glfw", "glew")
```

## Build Examples

To build a specific example:
```bash
xmake b opengl-example
xmake b sfml-example
xmake b sdl-example
xmake b raylib-example
```

## Project Structure
```
shimera/
├── include/                # Public headers
│   ├── backend/
│   │   ├── raylib/
│   │   └── sfml/
│   ├── effects/
│   ├── uniform/
│   └── shimera.h
├── src/                    # Source files
│   ├── backend/
│   │   ├── raylib/
│   │   └── sfml/
│   └── effects/
├── res/                    # Runtime resources (shaders)
│   └── shader/
│       └── postprocessing/
├── examples/
│   ├── opengl/
│   ├── raylib/
│   ├── sdl/
│   └── sfml/
├── build/              # Build artifacts
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
