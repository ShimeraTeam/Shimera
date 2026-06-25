#include "backend/raylib/RaylibPostProcessor.hpp"
#include "backend/raylib/RaylibShader.hpp"
#include "backend/ITexture.hpp"
#include <GL/glew.h>
#include <glUtils.h>
#include <iostream>
#include <array>
#include <stdexcept>

using shimera::IShader;
using shimera::ITexture;
using shimera::RaylibPostProcessor;
using shimera::RaylibShader;
using shimera::UniformValue;

RaylibPostProcessor::RaylibPostProcessor()
    : m_vao(0), m_vbo(0), m_ebo(0), m_shader(nullptr) {
    initializeQuad();
}

RaylibPostProcessor::~RaylibPostProcessor() {
    cleanupQuad();
    cleanupShader();
}

void RaylibPostProcessor::initializeQuad() {
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

    // Generate and bind VAO
    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glGenBuffers(1, &m_vbo));
    GLC(glGenBuffers(1, &m_ebo));

    GLC(glBindVertexArray(m_vao));

    // Upload vertex data
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GLC(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert.data(), GL_STATIC_DRAW));

    // Upload index data
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices.data(), GL_STATIC_DRAW));

    // Position attribute (location = 0)
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr));
    GLC(glEnableVertexAttribArray(0));

    // Texture coordinate attribute (location = 1)
    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)))); // NOLINT(performance-no-int-to-ptr)
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
        std::cerr << "Failed to load shader: " << e.what() << '\n';
        throw;
    }
}

void RaylibPostProcessor::render(ITexture& texture) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded for post-processing!" << '\n';
        return;
    }

    // Disable SFML's internal vertex array to avoid conflicts
    GLC(glBindVertexArray(0));

    m_shader->bind();
    GLC(glBindVertexArray(m_vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture.getNativeHandle()));

    for (const auto& tex : m_extraTextures) {
        GLC(glActiveTexture(GL_TEXTURE0 + tex.unit));
        GLC(glBindTexture(GL_TEXTURE_2D, tex.handle));
    }

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    for (const auto& tex : m_extraTextures) {
        GLC(glActiveTexture(GL_TEXTURE0 + tex.unit));
        GLC(glBindTexture(GL_TEXTURE_2D, 0));
    }

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, 0));

    GLC(glBindVertexArray(0));
    m_shader->unbind();

    m_extraTextures.clear();
}

void RaylibPostProcessor::addInputTexture(const std::string& uniformName, ITexture& texture, unsigned int unit) {
    setUniform(uniformName, static_cast<int>(unit));
    m_extraTextures.push_back({.unit=unit, .handle=texture.getNativeHandle()});
}

void RaylibPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot set uniform!" << '\n';
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
        std::cerr << "Error: No shader loaded, cannot bind!" << '\n';
        return;
    }
    m_shader->bind();
}
