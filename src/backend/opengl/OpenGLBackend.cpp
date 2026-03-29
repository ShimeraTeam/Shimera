#include "backend/opengl/OpenGLBackend.hpp"
#include "backend/opengl/OpenGLFramebuffer.hpp"
#include "backend/opengl/OpenGLTexture.hpp"

#include <stdexcept>

IFrameBuffer* OpenGLBackend::createFrameBuffer(int width, int height) {
    return new OpenGLFramebuffer(width, height);
}

IPostProccessor* OpenGLBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
    (void)vert;
    (void)frag;
    throw std::runtime_error("OpenGL post-processor creation is not implemented yet");
}

ITexture* OpenGLBackend::createTexture(const int width, const int height) {
    return new OpenGLTexture(width, height);
}

IShader* OpenGLBackend::createShader(const std::string& vert, const std::string& frag) {
    (void)vert;
    (void)frag;
    throw std::runtime_error("OpenGL shader creation is not implemented yet");
}
