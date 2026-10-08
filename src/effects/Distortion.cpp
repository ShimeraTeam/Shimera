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

#include "Distortion.hpp"

#include "Resources.inl"

using shimera::Distortion;

Distortion::Distortion(const float noiseScale, const float distortionStrength, const float timeScale)
    : m_noiseScale(noiseScale), m_distortionStrength(distortionStrength), m_timeScale(timeScale) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"), generatedShader("distortion.frag"));
}

void Distortion::updateUniforms() {
    // distortion.frag predates the u_ naming convention the other shaders use
    // so names below are what the GLSL actually declares. (We have to change that ofc)
    m_processor->setUniform("time", m_time);
    m_processor->setUniform("noiseScale", m_noiseScale);
    m_processor->setUniform("distortionStrength", m_distortionStrength);
    m_processor->setUniform("timeScale", m_timeScale);
}

Distortion& Distortion::withTime(const float time) {
    m_time = time;
    return *this;
}

Distortion& Distortion::withNoiseScale(const float noiseScale) {
    m_noiseScale = noiseScale;
    return *this;
}

Distortion& Distortion::withDistortionStrength(const float distortionStrength) {
    m_distortionStrength = distortionStrength;
    return *this;
}

Distortion& Distortion::withTimeScale(const float timeScale) {
    m_timeScale = timeScale;
    return *this;
}

float Distortion::getTime() const {
    return m_time;
}

float Distortion::getNoiseScale() const {
    return m_noiseScale;
}

float Distortion::getDistortionStrength() const {
    return m_distortionStrength;
}

float Distortion::getTimeScale() const {
    return m_timeScale;
}
