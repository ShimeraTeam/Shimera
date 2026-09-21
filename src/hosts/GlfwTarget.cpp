#include "GlfwTarget.hpp"

#include <GL/glew.h>

#include "GL/GLError.hpp"

using shimera::GlfwTarget;

GlfwTarget::GlfwTarget(const int width, const int height, const bool samplableDepth)
    : m_framebuffer(width, height, samplableDepth) {}

void GlfwTarget::begin() {
    m_previousFramebuffer = currentDrawFramebuffer();
    GLC(glGetIntegerv(GL_VIEWPORT, m_previousViewport));

    m_framebuffer.bind();
    GLC(glViewport(0, 0, m_framebuffer.width(), m_framebuffer.height()));
}

void GlfwTarget::end() {
    bindFramebuffer(m_previousFramebuffer);
    GLC(glViewport(m_previousViewport[0], m_previousViewport[1],
                   m_previousViewport[2], m_previousViewport[3]));
}

void GlfwTarget::resize(const int width, const int height) {
    m_framebuffer.resize(width, height);
}
