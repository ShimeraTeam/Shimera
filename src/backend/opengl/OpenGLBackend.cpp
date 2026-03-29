#include "backend/opengl/OpenGLBackend.hpp"
#include "backend/opengl/OpenGLFramebuffer.hpp"
#include "backend/opengl/OpenGLPostProcessor.hpp"
#include "backend/opengl/OpenGLShader.hpp"
#include "backend/opengl/OpenGLTexture.hpp"

#include <stdexcept>

IFrameBuffer* OpenGLBackend::createFrameBuffer(int width, int height) {
    return new OpenGLFramebuffer(width, height);
}

IPostProccessor* OpenGLBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
    auto processor = new OpenGLPostProcessor();
    processor->setShader(vert, frag);
    return processor;
}

ITexture* OpenGLBackend::createTexture(const int width, const int height) {
    return new OpenGLTexture(width, height);
}

IShader* OpenGLBackend::createShader(const std::string& vert, const std::string& frag) {
    auto shader = new OpenGLShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}
