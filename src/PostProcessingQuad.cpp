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

//
// Created by leoma on 24/11/2025.
//

#include <PostProcessingQuad.hpp>

#include <iostream>
#include <array>
#include <GL/glew.h>

#include <glUtils.h>

using shimera::PostProcessingQuad;


PostProcessingQuad::PostProcessingQuad(const std::string& vertPath,
                                       const std::string& fragPath) {
    const std::array<float, 16> quadVert = {
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
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices),
        quadIndices.data(), GL_STATIC_DRAW));

    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), (void*)nullptr));
    GLC(glEnableVertexAttribArray(0));

    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)))); // NOLINT(performance-no-int-to-ptr)
    GLC(glEnableVertexAttribArray(1));

    GLC(glBindVertexArray(0));

    const ShaderProgramSource source = parseShader(vertPath, fragPath);
    m_shader = createShader(source.vertex, source.fragment);
    if (m_shader == 0) {
        std::cerr << "Failed to create post-processing shader!" << '\n';
        return;
    }

    GLC(glUseProgram(m_shader));
    GLC(const int location = glGetUniformLocation(m_shader, "u_screenTexture"));
    ASSERT(location != -1);
    GLC(glUniform1i(location, 0));
}

PostProcessingQuad::~PostProcessingQuad() {
    GLC(glDeleteVertexArrays(1, &m_vao));
    GLC(glDeleteBuffers(1, &m_vbo));
    GLC(glDeleteBuffers(1, &m_ebo));
    GLC(glDeleteProgram(m_shader));
}

void PostProcessingQuad::render(unsigned int texture) const {
    GLC(glUseProgram(m_shader));
    GLC(glBindVertexArray(m_vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture));

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    GLC(glBindVertexArray(0));
}

unsigned int PostProcessingQuad::getShader() const {
    return m_shader;
}

void PostProcessingQuad::bindShader() const {
    GLC(glUseProgram(m_shader));
}
