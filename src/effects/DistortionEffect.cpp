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

#include "effects/DistortionEffect.hpp"

using shimera::DistortionEffect;
using shimera::IBackend;
using shimera::IPostProcessor;


DistortionEffect::DistortionEffect(IBackend* backend)
    : DistortionEffect(backend, 0.0f, 3.0f, 0.13f, 0.1f) {}

DistortionEffect::DistortionEffect(IBackend* backend, const float time, const float noiseScale,
    const float distortionStrength, const float timeScale)
    : m_uTime(time), m_uNoiseScale(noiseScale), m_uDistortionStrength(distortionStrength), m_uTimeScale(timeScale) {
     m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/distortion.frag"
        )
     );

}

void DistortionEffect::updateUniforms() {
    m_processor->setUniform("time", m_uTime);
    m_processor->setUniform("noiseScale", m_uNoiseScale);
    m_processor->setUniform("distortionStrength", m_uDistortionStrength);
    m_processor->setUniform("timeScale", m_uTimeScale);
}

DistortionEffect& DistortionEffect::withTime(float time) {
    m_uTime = time;
    return *this;
}

DistortionEffect& DistortionEffect::withNoiseScale(float noiseScale) {
    m_uNoiseScale = noiseScale;
    return *this;
}

DistortionEffect& DistortionEffect::withDistortionStrength(float distortionStrength) {
    m_uDistortionStrength = distortionStrength;
    return *this;
}

DistortionEffect& DistortionEffect::withTimeScale(float timeScale) {
    m_uTimeScale = timeScale;
    return *this;
}
