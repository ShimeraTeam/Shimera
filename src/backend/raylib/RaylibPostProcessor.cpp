#include "backend/raylib/RaylibPostProcessor.hpp"
#include "backend/raylib/RaylibShader.hpp"
#include "backend/ITexture.hpp"
#include <GL/glew.h>
#include <glUtils.h>
#include <iostream>
#include <stdexcept>

RaylibPostProcessor::RaylibPostProcessor() 
    : m_vao(0), m_vbo(0), m_ebo(0), m_shader(nullptr) {
    initializeQuad();
}

RaylibPostProcessor::~RaylibPostProcessor() {
    cleanupQuad();
    cleanupShader();
}

void RaylibPostProcessor::initializeQuad() {
    constexpr float quadVert[] = {
        // positions   // texCoords (uv)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    const unsigned int quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Generate and bind VAO
    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glGenBuffers(1, &m_vbo));
    GLC(glGenBuffers(1, &m_ebo));

    GLC(glBindVertexArray(m_vao));

    // Upload vertex data
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GLC(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert, GL_STATIC_DRAW));

    // Upload index data
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW));

    // Position attribute (location = 0)
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GLC(glEnableVertexAttribArray(0));

    // Texture coordinate attribute (location = 1)
    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    GLC(glEnableVertexAttribArray(1));

    GLC(glBindVertexArray(0));

    if (m_vao == 0) {
        throw std::runtime_error("Failed to create VAO for post-processing quad");
    }
}

void RaylibPostProcessor::cleanupQuad() {
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

void RaylibPostProcessor::cleanupShader() {
    m_shader.reset();
}

void RaylibPostProcessor::setShader(const std::string& vert, const std::string& frag) {
    auto newShader = std::make_unique<RaylibShader>();
    
    try {
        newShader->loadFromFiles(vert, frag);
        m_shader = std::move(newShader);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << std::endl;
        throw;
    }
}

void RaylibPostProcessor::render(ITexture& texture) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded for post-processing!" << std::endl;
        return;
    }

    // Disable SFML's internal vertex array to avoid conflicts
    GLC(glBindVertexArray(0));

    m_shader->bind();
    GLC(glBindVertexArray(m_vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture.getNativeHandle()));

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    GLC(glBindVertexArray(0));
    m_shader->unbind();
}

void RaylibPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot set uniform!" << std::endl;
        return;
    }
    
    m_shader->setUniform(name, value);
}

IShader& RaylibPostProcessor::getShader() {
    if (!m_shader) {
        throw std::runtime_error("No shader loaded!");
    }
    return *m_shader;
}

void RaylibPostProcessor::bindShader() {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot bind!" << std::endl;
        return;
    }
    m_shader->bind();
}
