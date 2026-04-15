#include "backend/BackendFactory.hpp"

#ifdef SHIMERA_BACKEND_OPENGL
    #include "backend/opengl/OpenGLBackend.hpp"
#endif

#ifdef SHIMERA_BACKEND_SFML
    #include "backend/sfml/SFMLBackend.hpp"
#endif

#ifdef SHIMERA_BACKEND_RAYLIB
    #include "backend/raylib/RaylibBackend.hpp"
#endif

IBackend* BackendFactory::create() {
#ifdef SHIMERA_BACKEND_OPENGL
    return new OpenGLBackend();
#elif defined(SHIMERA_BACKEND_SFML)
    return new SFMLBackend();
#elif defined(SHIMERA_BACKEND_SDL)
    // return new SDLBackend();
    return nullptr;
#elif defined(SHIMERA_BACKEND_RAYLIB)
    return new RaylibBackend();
#else
    #error "No backend defined. Define SHIMERA_BACKEND_OPENGL, SHIMERA_BACKEND_SFML, or SHIMERA_BACKEND_SDL"
#endif
}
