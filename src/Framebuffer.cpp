#include <Framebuffer.hpp>

#include <iostream>
#include <ostream>
#include <GL/glew.h>

#include <glUtils.h>

using shimera::Framebuffer;


Framebuffer::Framebuffer(int w, int h)
    : m_width(w), m_height(h),m_fbo(0), m_texture(0), m_rbo(0) {
    // Make the framebuffer
    GLC(glGenFramebuffers(1, &m_fbo));
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    // Make the texture that the framebuffer will render to
    GLC(glGenTextures(1, &m_texture));
    GLC(glBindTexture(GL_TEXTURE_2D, m_texture));
    GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,
        0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, m_texture, 0));

    /* Make the renderbuffer object that will hold the depth and stencil buffers
    for the framebuffer to use */
    GLC(glGenRenderbuffers(1, &m_rbo));
    GLC(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
    GLC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        w, h));
    GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, m_rbo));

    GLC(if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;)

    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer() {
    GLC(glDeleteFramebuffers(1, &m_fbo));
    GLC(glDeleteTextures(1, &m_texture));
    GLC(glDeleteRenderbuffers(1, &m_rbo));
}

void Framebuffer::bind() const {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void Framebuffer::unbind() const {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int Framebuffer::getTexture() const {
    return m_texture;
}
