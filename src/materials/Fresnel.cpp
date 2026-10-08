// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
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

#include "Fresnel.hpp"

#include "Resources.inl"

using shimera::Fresnel;
using shimera::Vec3;

Fresnel::Fresnel(const Vec3<float> color, const float power,
                 const float reflectance, const float intensity)
    : m_color(color), m_power(power), m_reflectance(reflectance), m_intensity(intensity) {
    m_shader = GLShader::fromFiles(materialShader("fresnel.vert"), materialShader("fresnel.frag"));
}

void Fresnel::updateUniforms() {
    m_shader->setUniform("u_color", m_color);
    m_shader->setUniform("u_power", m_power);
    m_shader->setUniform("u_reflectance", m_reflectance);
    m_shader->setUniform("u_intensity", m_intensity);
}

Fresnel& Fresnel::withColor(const Vec3<float> color) {
    m_color = color;
    return *this;
}

Fresnel& Fresnel::withPower(const float power) {
    m_power = power;
    return *this;
}

Fresnel& Fresnel::withReflectance(const float reflectance) {
    m_reflectance = reflectance;
    return *this;
}

Fresnel& Fresnel::withIntensity(const float intensity) {
    m_intensity = intensity;
    return *this;
}
