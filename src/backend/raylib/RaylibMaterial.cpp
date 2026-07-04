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

#include "backend/raylib/RaylibMaterial.hpp"

#include "backend/raylib/converts/RaylibConvert.hpp"

using shimera::RaylibMaterial;
using shimera::UniformValue;

RaylibMaterial::RaylibMaterial(const std::string& v, const std::string& f) {
    m_shader = LoadShader(v.c_str(), f.c_str());
    bindShimeraConventions();
}
RaylibMaterial::~RaylibMaterial() {
    UnloadShader(m_shader);
}

void RaylibMaterial::bindShimeraConventions() {
    m_shader.locs[SHADER_LOC_VERTEX_POSITION] = GetShaderLocationAttrib(m_shader, "aPos");
    m_shader.locs[SHADER_LOC_VERTEX_NORMAL] = GetShaderLocationAttrib(m_shader, "aNormal");
    m_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(m_shader, "u_model");
    m_shader.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(m_shader, "u_view");
    m_shader.locs[SHADER_LOC_MATRIX_PROJECTION] = GetShaderLocation(m_shader, "u_projection");
}

int RaylibMaterial::locOf(const std::string& name) {
    auto it = m_locs.find(name);
    if (it != m_locs.end()) return it->second;
    int loc = GetShaderLocation(m_shader, name.c_str());
    m_locs[name] = loc;
    return loc;
}

void RaylibMaterial::setUniform(const std::string& name, const UniformValue& value) {
    const int loc = locOf(name);
    if (loc < 0) return;
    std::visit([&](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, float>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_FLOAT);
        else if constexpr (std::is_same_v<T, int>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_INT);
        else if constexpr (std::is_same_v<T, Vec2<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC2);
        else if constexpr (std::is_same_v<T, Vec3<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC3);
        else if constexpr (std::is_same_v<T, Vec4<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC4);
        else if constexpr (std::is_same_v<T, Mat4>)
            SetShaderValueMatrix(m_shader, loc, toRaylibMatrix(v));
    }, value);
}

Shader RaylibMaterial::shader() const {
    return m_shader;
}
