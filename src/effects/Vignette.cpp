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

#include "Vignette.hpp"

#include "Resources.inl"

using shimera::Vignette;

Vignette::Vignette(const float strength, const float radius, const float gap,
                   const Vec4<float> color, const bool rounded)
    : m_strength(strength), m_radius(radius), m_gap(gap),
      m_color(color), m_rounded(rounded ? 1 : 0) {
    // The only diff between Slang shader and hand written shaders.
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("vignette.frag"));
}

void Vignette::updateUniforms() {
    m_processor->setUniform("u_strength", m_strength);
    m_processor->setUniform("u_radius", m_radius);
    m_processor->setUniform("u_gap", m_gap);
    m_processor->setUniform("u_color", m_color);
    m_processor->setUniform("u_isRounded", m_rounded);
    m_processor->setUniform("u_resolution", m_resolution);
}

Vignette& Vignette::withStrength(const float strength) {
    m_strength = strength;
    return *this;
}

Vignette& Vignette::withRadius(const float radius) {
    m_radius = radius;
    return *this;
}

Vignette& Vignette::withGap(const float gap) {
    m_gap = gap;
    return *this;
}

Vignette& Vignette::withColor(const Vec4<float> color) {
    m_color = color;
    return *this;
}

Vignette& Vignette::withRounded(const bool rounded) {
    m_rounded = rounded ? 1 : 0;
    return *this;
}
