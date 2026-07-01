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

#include "effects/ChromaticAberration.hpp"

using shimera::ChromaticAberrationEffect;
using shimera::IBackend;
using shimera::IPostProcessor;

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend)
    : ChromaticAberrationEffect(backend, 1.0f, false, 2.0f, 20) {}

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend, const float strength, const bool radius,
    const float contrast, const int samples)
    : m_uStrength(strength), m_uRadius(radius), m_uContrast(contrast), m_uSamples(samples) {
     m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/chromatic_aberration.frag"
        )
     );
}

void ChromaticAberrationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
    m_processor->setUniform("u_radius", m_uRadius ? 1 : 0);
    m_processor->setUniform("u_contrast", m_uContrast);
        m_processor->setUniform("u_samples", m_uSamples);
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withStrength(float s) {
    m_uStrength = s;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withRadius(bool r) {
    m_uRadius = r;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withContrast(float c) {
    m_uContrast = c;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withSamples(int s) {
    m_uSamples = s;
    return *this;
}
