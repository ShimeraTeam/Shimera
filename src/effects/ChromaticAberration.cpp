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

#include "ChromaticAberration.hpp"

#include "../GL/GLPostProcessor.hpp"
#include "../Resources.inl"

using shimera::ChromaticAberration;

ChromaticAberration::ChromaticAberration(const float strength, const bool radius,
                                         const float contrast, const int samples)
    : m_uStrength(strength), m_uRadius(radius), m_uContrast(contrast), m_uSamples(samples) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("chromatic_aberration.frag"));
}

void ChromaticAberration::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
    m_processor->setUniform("u_radius", m_uRadius);
    m_processor->setUniform("u_contrast", m_uContrast);
    m_processor->setUniform("u_samples", m_uSamples);
}

ChromaticAberration& ChromaticAberration::withStrength(const float s) {
    m_uStrength = s;
    return *this;
}

ChromaticAberration& ChromaticAberration::withRadius(const bool r) {
    m_uRadius = r;
    return *this;
}

ChromaticAberration& ChromaticAberration::withContrast(const float c) {
    m_uContrast = c;
    return *this;
}

ChromaticAberration& ChromaticAberration::withSamples(const int s) {
    m_uSamples = s;
    return *this;
}