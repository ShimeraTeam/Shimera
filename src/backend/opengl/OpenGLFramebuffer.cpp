#include <GL/glew.h>
#include <glUtils.h>

#include <stdexcept>

#include "backend/opengl/OpenGLFramebuffer.hpp"

OpenGLFramebuffer::OpenGLFramebuffer(const int width, const int height)
    : fbo(0), rbo(0), texture(nullptr), m_width(width), m_height(height) {
    OpenGLFramebuffer::resize(width, height);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    if (rbo != 0) {
        GLC(glDeleteRenderbuffers(1, &rbo));
    }
    if (fbo != 0) {
        GLC(glDeleteFramebuffers(1, &fbo));
    }
}

void OpenGLFramebuffer::bind() {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

void OpenGLFramebuffer::unbind() {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLFramebuffer::clear(const shimera::Color color) {
    GLC(glClearColor(color.r, color.g, color.b, color.a));
    GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

ITexture& OpenGLFramebuffer::getTexture() {
    return *texture;
}

void OpenGLFramebuffer::resize(const int width, const int height) {
    m_width = width;
    m_height = height;

    if (fbo == 0) {
        GLC(glGenFramebuffers(1, &fbo));
    }
    if (rbo == 0) {
        GLC(glGenRenderbuffers(1, &rbo));
    }

    texture = std::make_unique<OpenGLTexture>(width, height);

    GLC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, texture->getNativeHandle(), 0));

    GLC(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    GLC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        throw std::runtime_error("OpenGL framebuffer is not complete");
    }

    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

int OpenGLFramebuffer::getWidth() const {
    return m_width;
}

int OpenGLFramebuffer::getHeight() const {
    return m_height;
}

void* OpenGLFramebuffer::getNativeRenderTarget() {
    return nullptr;
}
