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

#include "Bloom.hpp"

#include "Resources.inl"

using shimera::GLTexture;
using shimera::Bloom;

Bloom::Bloom(const float threshold, const float knee, const float intensity,
                   const float blurSigma, const int blurSamples)
    : m_threshold(threshold), m_knee(knee), m_intensity(intensity),
      m_blurSigma(blurSigma), m_blurSamples(blurSamples) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("bloom.frag"));
}

void Bloom::resize(const int width, const int height) {
    Effect::resize(width, height);
    if (m_brightBlurredH)
        m_brightBlurredH->resize(width, height);
    else
        m_brightBlurredH = std::make_unique<GLFramebuffer>(width, height);
}

void Bloom::render(const GLTexture& input) {
    if (!m_enabled)
        return;
    renderImpl(input, currentDrawFramebuffer());
}

void Bloom::render(const GLTexture& input, GLFramebuffer& target) {
    if (!m_enabled)
        return;
    renderImpl(input, target.handle());
}

void Bloom::renderImpl(const GLTexture& input, const uint32_t target) {
    updateUniforms();

    // Pass 1. bright-pass + horizontal blur into the intermediate.
    m_processor->setUniform("u_mode", 0);
    m_brightBlurredH->bind();
    m_processor->render(input);

    // Pass 2. vertical blur of that, composited over the original scene.
    m_processor->setUniform("u_mode", 1);
    m_processor->addInputTexture("u_brightBlurredH", m_brightBlurredH->color(), 1);
    bindFramebuffer(target);
    m_processor->render(input);
}

void Bloom::updateUniforms() {
    m_processor->setUniform("u_threshold", m_threshold);
    m_processor->setUniform("u_knee", m_knee);
    m_processor->setUniform("u_intensity", m_intensity);
    m_processor->setUniform("u_sigma", m_blurSigma);
    m_processor->setUniform("u_samples", m_blurSamples);
    m_processor->setUniform("u_resolution", m_resolution);
}

Bloom& Bloom::withThreshold(const float threshold) {
    m_threshold = threshold;
    return *this;
}

Bloom& Bloom::withKnee(const float knee) {
    m_knee = knee;
    return *this;
}

Bloom& Bloom::withIntensity(const float intensity) {
    m_intensity = intensity;
    return *this;
}

Bloom& Bloom::withBlurSigma(const float sigma) {
    m_blurSigma = sigma;
    return *this;
}

Bloom& Bloom::withBlurSamples(const int samples) {
    m_blurSamples = samples;
    return *this;
}
