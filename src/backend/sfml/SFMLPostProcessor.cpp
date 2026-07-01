// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "backend/sfml/SFMLPostProcessor.hpp"
#include "backend/sfml/SFMLShader.hpp"
#include "backend/ITexture.hpp"
#include <GL/glew.h>
#include <glUtils.h>
#include <iostream>
#include <stdexcept>
#include <array>

using shimera::IShader;
using shimera::ITexture;
using shimera::SFMLPostProcessor;
using shimera::SFMLShader;
using shimera::UniformValue;

SFMLPostProcessor::SFMLPostProcessor()
    : m_vao(0), m_vbo(0), m_ebo(0), m_shader(nullptr) {
    initializeQuad();
}

SFMLPostProcessor::~SFMLPostProcessor() {
    cleanupQuad();
    cleanupShader();
}

void SFMLPostProcessor::initializeQuad() {
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

void SFMLPostProcessor::cleanupQuad() {
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

void SFMLPostProcessor::cleanupShader() {
    m_shader.reset();
}

void SFMLPostProcessor::setShader(const std::string& vert, const std::string& frag) {
    auto newShader = std::make_unique<SFMLShader>();
    
    try {
        newShader->loadFromFiles(vert, frag);
        m_shader = std::move(newShader);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << '\n';
        throw;
    }
}

void SFMLPostProcessor::render(ITexture& texture) {
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

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    GLC(glBindVertexArray(0));
    m_shader->unbind();
}

void SFMLPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot set uniform!" << '\n';
        return;
    }
    
    m_shader->setUniform(name, value);
}

IShader& SFMLPostProcessor::getShader() {
    if (!m_shader) {
        throw std::runtime_error("No shader loaded!");
    }
    return *m_shader;
}

void SFMLPostProcessor::bindShader() {
    if (!m_shader) {
        std::cerr << "Error: No shader loaded, cannot bind!" << '\n';
        return;
    }
    m_shader->bind();
}


