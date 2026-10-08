#include "GLFramebuffer.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

#include <stdexcept>
#include <string>

using shimera::GLFramebuffer;

namespace {

std::string statusName(const GLenum status) {
    switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "INCOMPLETE_DRAW_BUFFER";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "INCOMPLETE_READ_BUFFER";
        case GL_FRAMEBUFFER_UNSUPPORTED: return "UNSUPPORTED";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "INCOMPLETE_MULTISAMPLE";
        default: return "0x" + std::to_string(status);
    }
}

} // namespace

uint32_t shimera::currentDrawFramebuffer() {
    int current = 0;
    GLC(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current));
    return static_cast<uint32_t>(current);
}

void shimera::bindFramebuffer(const uint32_t handle) {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, handle));
}

void shimera::clearDepth() {
    // glClear ignores the depth buffer entirely when depth writes are masked off, and we
    // cannot know what the host left the mask at.
    GLboolean depthMask = GL_TRUE;
    GLC(glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask));

    GLC(glDepthMask(GL_TRUE));
    GLC(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GLC(glDepthMask(depthMask));
}

GLFramebuffer::GLFramebuffer(const int width, const int height, const bool samplableDepth)
    : m_samplableDepth(samplableDepth) {
    resize(width, height);
}

GLFramebuffer::~GLFramebuffer() {
    // Textures go first, they are attachments of the framebuffer below.
    m_color.reset();
    m_depth.reset();
    if (m_rbo != 0) {
        glDeleteRenderbuffers(1, &m_rbo);
    }
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }
}

void GLFramebuffer::bind() const {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void GLFramebuffer::clear(const Color color) {
    GLC(glClearColor(color.m_r, color.m_g, color.m_b, color.m_a));
    GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void GLFramebuffer::resize(const int width, const int height) {
    if (width <= 0 || height <= 0)
        throw std::runtime_error("shimera: framebuffer size must be positive, got "
            + std::to_string(width) + "x" + std::to_string(height));

    m_width = width;
    m_height = height;

    /* resize() runs outside any render section (window events, pipeline setup), so no
     * outer guard is protecting the host here. So we put a GLStateGuard here. */
    const GLStateGuard guard;

    if (m_fbo == 0) {
        GLC(glGenFramebuffers(1, &m_fbo));
    }
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    m_color.emplace(width, height, GLTexture::Format::Color);
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, m_color->getHandle(), 0));

    if (m_samplableDepth) {
        m_depth.emplace(width, height, GLTexture::Format::Depth);
        GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, m_depth->getHandle(), 0));
    } else {
        if (m_rbo == 0) {
            GLC(glGenRenderbuffers(1, &m_rbo));
        }
        GLC(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
        GLC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, m_rbo));
    }

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("shimera: framebuffer incomplete (" + statusName(status) + ")");
}
