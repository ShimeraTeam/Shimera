# Shimera

A C++ library for Shaders, designed to work with SFML or pure OpenGL projects.

## Requirements

- **C++17** or higher
- **OpenGL 3.3+**
- **GLEW** (OpenGL Extension Wrangler Library)
- **xmake**

## Installation

### Quick Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/shimera.git
cd shimera

# Build the library (static by default)
xmake

# Install to system (optional)
sudo xmake install
```

### Build Options

```bash
# Build static library (default)
xmake

# Build shared library
xmake f --shared=y
xmake

# Build in release mode
xmake f -m release
xmake
```

## Using Shimera in Your Project

### Option 1: System Installation

After building, install the library system-wide:

```bash
sudo xmake install
```

Then in your project's `xmake.lua`:

```lua
add_requires("glew")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    -- Link with shimera
    add_includedirs("/usr/local/include")
    add_linkdirs("/usr/local/lib")
    add_links("shimera")
```

### Option 2: Local Library

Copy the built library files to your project:

#### Using Static Library (.a)

```bash
# After building shimera (static mode)
cp -r shimera/include /path/to/your/project/
cp shimera/build/linux/x86_64/release/libshimera.a /path/to/your/project/lib/
```

Then in your `xmake.lua`:

```lua
target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    -- Use local shimera (static)
    add_includedirs("include")
    add_linkdirs("lib")
    add_links("shimera")
```

#### Using Shared Library (.so)

```bash
# Build shimera in shared mode
cd shimera
xmake f --shared=y
xmake

# Copy files to your project
cp -r shimera/include /path/to/your/project/
cp shimera/build/linux/x86_64/release/libshimera.so /path/to/your/project/lib/
```

Then in your `xmake.lua`:

```lua
target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glew")

    -- Use local shimera (shared)
    add_includedirs("include")
    add_linkdirs("lib")
    add_links("shimera")
    add_rpathdirs("lib")  -- Add runtime path for .so
```

**Note:** With shared libraries, you need to ensure `libshimera.so` is accessible at runtime:
- Option 1: Add `add_rpathdirs()` in xmake (recommended)
- Option 2: Copy `libshimera.so` next to your executable
- Option 3: Add the library path to `LD_LIBRARY_PATH`

### Option 3: Submodule (Recommended for Development)

```bash
cd your-project
git submodule add https://github.com/yourusername/shimera.git libs/shimera
```

In your `xmake.lua`:

```lua
add_requires("glew")

-- Include shimera library
includes("libs/shimera")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_deps("shimera")  -- Link with shimera
    add_packages("glew")
```

## Project Structure

```
Shimera/
├── include/              # Public headers
│   ├── Framebuffer.h
│   ├── PostProcessingQuad.h
│   ├── glUtils.h
│   ├── shimera.h        # Main header (includes all)
│   └── uniform/
│       ├── Uniform.hpp
│       └── Vec4.hpp
├── src/                 # Implementation files
│   ├── Framebuffer.cpp
│   ├── PostProcessingQuad.cpp
│   └── glUtils.cpp
├── res/                 # Shader resources
│   └── shader/
│       └── postprocessing/
├── examples/            # Example projects
│   ├── sfml/
│   ├── opengl/
│   └── sdl/
├── build/              # Build artifacts
│   └── linux/x86_64/release/
│       ├── libshimera.a   # Static library
│       └── libshimera.so  # Shared library (if built)
└── xmake.lua           # Build configuration
```