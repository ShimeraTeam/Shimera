#include "GLStateGuard.hpp"

#include <GL/glew.h>

#include "GL/GLError.hpp"

using shimera::GLStateGuard;

namespace {

void setEnableGL(const GLenum cap, const unsigned char on) {
    if (on) {
        GLC(glEnable(cap));
    } else {
        GLC(glDisable(cap));
    }
}

} // unnamed namespace

bool GLStateGuard::s_enabled = true;

void GLStateGuard::setEnabled(const bool enabled) {
    s_enabled = enabled;
}

bool GLStateGuard::isEnabled() {
    return s_enabled;
}

GLStateGuard::GLStateGuard() {
    m_captured = s_enabled;
    if (m_captured)
        capture();
}

GLStateGuard::~GLStateGuard() {
    if (m_captured)
        restore();
}

void GLStateGuard::capture() {
    m_blend = glIsEnabled(GL_BLEND);
    m_cullFace = glIsEnabled(GL_CULL_FACE);
    m_depthTest = glIsEnabled(GL_DEPTH_TEST);
    m_scissorTest = glIsEnabled(GL_SCISSOR_TEST);

    GLC(glGetBooleanv(GL_DEPTH_WRITEMASK, &m_depthMask));
    GLC(glGetIntegerv(GL_DEPTH_FUNC, &m_depthFunc));

    GLC(glGetIntegerv(GL_BLEND_SRC_RGB, &m_blendSrcRGB));
    GLC(glGetIntegerv(GL_BLEND_DST_RGB, &m_blendDstRGB));
    GLC(glGetIntegerv(GL_BLEND_SRC_ALPHA, &m_blendSrcAlpha));
    GLC(glGetIntegerv(GL_BLEND_DST_ALPHA, &m_blendDstAlpha));
    GLC(glGetIntegerv(GL_BLEND_EQUATION_RGB, &m_blendEquationRGB));
    GLC(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &m_blendEquationAlpha));

    GLC(glGetIntegerv(GL_CURRENT_PROGRAM, &m_program));
    GLC(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_vertexArray));
    GLC(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_arrayBuffer));
    GLC(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_elementBuffer));
    GLC(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_drawFramebuffer));

    GLC(glGetIntegerv(GL_ACTIVE_TEXTURE, &m_activeTexture));
    for (int unit = 0; unit < kTextureUnits; ++unit) {
        GLC(glActiveTexture(GL_TEXTURE0 + unit));
        GLC(glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_texture2D[unit]));
    }
    GLC(glActiveTexture(static_cast<GLenum>(m_activeTexture)));

    GLC(glGetIntegerv(GL_VIEWPORT, m_viewport));
    GLC(glGetFloatv(GL_COLOR_CLEAR_VALUE, m_clearColor));
}

void GLStateGuard::restore() const {
    // Textures first, and leave the active unit as we found it.
    for (int unit = 0; unit < kTextureUnits; ++unit) {
        GLC(glActiveTexture(GL_TEXTURE0 + unit));
        GLC(glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(m_texture2D[unit])));
    }
    GLC(glActiveTexture(static_cast<GLenum>(m_activeTexture)));

    GLC(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(m_drawFramebuffer)));

    /* The element buffer binding belongs to the VAO, so the VAO has to be
     * bound before its element buffer is put back. */
    GLC(glBindVertexArray(static_cast<GLuint>(m_vertexArray)));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(m_elementBuffer)));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(m_arrayBuffer)));
    GLC(glUseProgram(static_cast<GLuint>(m_program)));

    GLC(glBlendFuncSeparate(static_cast<GLenum>(m_blendSrcRGB), static_cast<GLenum>(m_blendDstRGB),
                            static_cast<GLenum>(m_blendSrcAlpha), static_cast<GLenum>(m_blendDstAlpha)));
    GLC(glBlendEquationSeparate(static_cast<GLenum>(m_blendEquationRGB),
                                static_cast<GLenum>(m_blendEquationAlpha)));

    GLC(glDepthFunc(static_cast<GLenum>(m_depthFunc)));
    GLC(glDepthMask(m_depthMask));

    setEnableGL(GL_BLEND, m_blend);
    setEnableGL(GL_CULL_FACE, m_cullFace);
    setEnableGL(GL_DEPTH_TEST, m_depthTest);
    setEnableGL(GL_SCISSOR_TEST, m_scissorTest);

    GLC(glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]));
    GLC(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
}
