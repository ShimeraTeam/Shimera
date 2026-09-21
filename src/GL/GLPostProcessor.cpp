#include "GLPostProcessor.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

#include <array>
#include <stdexcept>

using shimera::GLPostProcessor;
using shimera::GLTexture;

GLPostProcessor::GLPostProcessor(const std::string& vertPath, const std::string& fragPath) {
    /* Creation binds a VAO, buffers and a program. So we have to guard the constructor,
     * because a host is free to cache GL state across our object creation, and
     * SDL's GL renderer even crashes if a draw finds an unknown VAO bound. */
    const GLStateGuard guard;
    initializeQuad();
    /* Throws with the shader name and the driver's log, let it propagate, the quad is
    * cleaned up by the destructor. */
    m_shader = GLShader::fromFiles(vertPath, fragPath);
}

GLPostProcessor::~GLPostProcessor() {
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
}

void GLPostProcessor::initializeQuad() {
    static constexpr std::array<float, 16> quadVert = {
        // position   // uv
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    static constexpr std::array<unsigned int, 6> quadIndices = {
        0, 1, 2,
        0, 2, 3
    };

    GLC(glGenVertexArrays(1, &m_vao));
    if (m_vao == 0)
        throw std::runtime_error("shimera: failed to create the post-processing VAO");

    GLC(glGenBuffers(1, &m_vbo));
    GLC(glGenBuffers(1, &m_ebo));

    GLC(glBindVertexArray(m_vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GLC(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert.data(), GL_STATIC_DRAW));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices.data(), GL_STATIC_DRAW));

    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        reinterpret_cast<void*>(2 * sizeof(float)))); // NOLINT(performance-no-int-to-ptr)
    GLC(glEnableVertexAttribArray(1));

    /* The element buffer binding is part of the VAO, so unbinding the VAO here is
    * recording state, not trashing it. */
    GLC(glBindVertexArray(0));
}

void GLPostProcessor::render(const GLTexture& input) {
    m_shader->bind();
    GLC(glBindVertexArray(m_vao));

    input.bind(0);
    for (const auto& [unit, handle] : m_extraTextures) {
        GLC(glActiveTexture(GL_TEXTURE0 + unit));
        GLC(glBindTexture(GL_TEXTURE_2D, handle));
    }

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    m_extraTextures.clear();
}

void GLPostProcessor::addInputTexture(const std::string& uniformName, const GLTexture& texture, const int unit) {
    setUniform(uniformName, unit);
    m_extraTextures.push_back({.unit = unit, .handle = texture.getHandle()});
}

void GLPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    m_shader->setUniform(name, value);
}
