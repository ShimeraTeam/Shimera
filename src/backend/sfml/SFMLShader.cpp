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

#include "backend/sfml/SFMLShader.hpp"
#include <GL/glew.h>
#include <glUtils.h>
#include <iostream>
#include <stdexcept>
#include <variant>
#include "uniform/Vec3.inl"
#include "uniform/Vec4.inl"

using shimera::SFMLShader;
using shimera::UniformValue;
using shimera::Vec2;
using shimera::Vec3;
using shimera::Vec4;

SFMLShader::SFMLShader() : m_programId(0) {}

SFMLShader::~SFMLShader() {
    if (m_programId != 0) {
        GLC(glDeleteProgram(m_programId));
    }
}

void SFMLShader::loadFromFiles(const std::string& vertPath, const std::string& fragPath) {
    const ShaderProgramSource source = parseShader(vertPath, fragPath);
    
    m_programId = createShader(source.vertex, source.fragment);
    
    if (m_programId == 0) {
        throw std::runtime_error("Failed to create shader program from files: " + vertPath + ", " + fragPath);
    }
    
    // Set up the texture sampler uniform (u_screenTexture) which is automatically bound to texture unit 0
    bind();
    const int location = getUniformLocation("u_screenTexture");
    if (location != -1) {
        GLC(glUniform1i(location, 0));
    }
    unbind();
}

void SFMLShader::bind() const {
    GLC(glUseProgram(m_programId));
}

void SFMLShader::unbind() const {
    GLC(glUseProgram(0));
}

void SFMLShader::setUniform(const std::string& name, const UniformValue& value) {
    const int location = getUniformLocation(name);
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader." << '\n';
        return;
    }
    
    bind();
    
    std::visit([location](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, float>) {
            GLC(glUniform1f(location, val));
        } else if constexpr (std::is_same_v<T, int>) {
            GLC(glUniform1i(location, val));
        } else if constexpr (std::is_same_v<T, Vec2<float>>) {
            GLC(glUniform2f(location, val.x, val.y));
        } else if constexpr (std::is_same_v<T, Vec3<float>>) {
            GLC(glUniform3f(location, val.x, val.y, val.z));
        } else if constexpr (std::is_same_v<T, Vec4<float>>) {
            GLC(glUniform4f(location, val.x, val.y, val.z, val.w));
        }
    }, value);
}

uint32_t SFMLShader::getNativeHandle() const {
    return m_programId;
}

int SFMLShader::getUniformLocation(const std::string& name) {
    // Cache checking
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }

    GLC(const int location = glGetUniformLocation(m_programId, name.c_str()));
    m_uniformCache[name] = location;
    
    return location;
}
