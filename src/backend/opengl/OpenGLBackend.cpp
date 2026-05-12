#include "backend/opengl/OpenGLBackend.hpp"
#include "backend/opengl/OpenGLFramebuffer.hpp"
#include "backend/opengl/OpenGLPostProcessor.hpp"
#include "backend/opengl/OpenGLShader.hpp"
#include "backend/opengl/OpenGLTexture.hpp"

#include <stdexcept>

using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::IShader;
using shimera::ITexture;
using shimera::OpenGLBackend;
using shimera::OpenGLFramebuffer;
using shimera::OpenGLPostProcessor;
using shimera::OpenGLShader;
using shimera::OpenGLTexture;

IFrameBuffer* OpenGLBackend::createFrameBuffer(int width, int height) {
    return new OpenGLFramebuffer(width, height);
}

IPostProcessor* OpenGLBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
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
