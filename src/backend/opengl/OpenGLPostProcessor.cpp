#include "backend/opengl/OpenGLPostProcessor.hpp"

#include <GL/glew.h>
#include <glUtils.h>

#include <iostream>
#include <stdexcept>
#include <array>

#include "backend/opengl/OpenGLShader.hpp"

using shimera::IPostProcessor;
using shimera::IShader;
using shimera::ITexture;
using shimera::OpenGLPostProcessor;
using shimera::OpenGLShader;

OpenGLPostProcessor::OpenGLPostProcessor()
    : m_vao(0), m_vbo(0), m_ebo(0), m_shader(nullptr) {
    initializeQuad();
}

OpenGLPostProcessor::~OpenGLPostProcessor() {
    cleanupQuad();
    cleanupShader();
}

void OpenGLPostProcessor::initializeQuad() {
    constexpr std::array<float, 16> quadVert = {
        // positions   // texCoords (uv)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    const std::array<unsigned int, 6> quadIndices = {
        0, 1, 2,
        0, 2, 3
    };

    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glGenBuffers(1, &m_vbo));
    GLC(glGenBuffers(1, &m_ebo));

    GLC(glBindVertexArray(m_vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GLC(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert.data(), GL_STATIC_DRAW));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices.data(), GL_STATIC_DRAW));

    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr));
    GLC(glEnableVertexAttribArray(0));

    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)))); // NOLINT(performance-no-int-to-ptr)
    GLC(glEnableVertexAttribArray(1));

    GLC(glBindVertexArray(0));

    if (m_vao == 0) {
        throw std::runtime_error("Failed to create VAO for post-processing quad");
    }
}

void OpenGLPostProcessor::cleanupQuad() {
    if (m_vao != 0) {
        GLC(glDeleteVertexArrays(1, &m_vao));
        m_vao = 0;
    }
    if (m_vbo != 0) {
        GLC(glDeleteBuffers(1, &m_vbo));
        m_vbo = 0;
    }
    if (m_ebo != 0) {
        GLC(glDeleteBuffers(1, &m_ebo));
        m_ebo = 0;
    }
}

void OpenGLPostProcessor::cleanupShader() {
    m_shader.reset();
}

void OpenGLPostProcessor::setShader(const std::string& vert, const std::string& frag) {
    auto newShader = std::make_unique<OpenGLShader>();

    try {
        newShader->loadFromFiles(vert, frag);
        m_shader = std::move(newShader);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << '\n';
        throw;
    }
}

void OpenGLPostProcessor::render(ITexture& texture) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded for post-processing!" << '\n';
        return;
    }

    GLC(glBindVertexArray(0));

    m_shader->bind();
    GLC(glBindVertexArray(m_vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture.getNativeHandle()));

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    GLC(glBindVertexArray(0));
    m_shader->unbind();
}

void OpenGLPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot set uniform!" << '\n';
        return;
    }

    m_shader->setUniform(name, value);
}

IShader& OpenGLPostProcessor::getShader() {
    if (!m_shader) {
        throw std::runtime_error("No shader loaded!");
    }
    return *m_shader;
}

void OpenGLPostProcessor::bindShader() {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot bind!" << '\n';
        return;
    }
    m_shader->bind();
}
