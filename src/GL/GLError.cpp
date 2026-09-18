#include "GLError.hpp"

#include <GL/glew.h>

#include <iostream>

void shimera::clearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {}
}

bool shimera::logGLCall(const char* function, const char* file, const int line) {
    while (const GLenum error = glGetError()) {
        std::cerr << "[OpenGL ERROR] (" << error << "): " << function
                  << " -> " << file << ":" << line << '\n';
        return false;
    }
    return true;
}
