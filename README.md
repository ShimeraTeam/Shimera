# Shimera

A C++ library for Shaders, designed to work with SFML, Raylib, or pure OpenGL projects.

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

Shimera is licensed under the ... License. See the [license.md](LICENSE) file for more details.