#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#if defined(_MSC_VER)
    #define DEBUGBREAK() __debugbreak()
#elif defined(__APPLE__) || defined(__linux__)
    #include <csignal>
    #define DEBUGBREAK() raise(SIGTRAP)
#else
    #define DEBUGBREAK() ((void)0)
#endif

#define ASSERT(x) if (!(x)) DEBUGBREAK();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
    public:
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};