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

#include "effects/HDRBloomEffect.hpp"
#include <GL/glew.h>

using shimera::IBackend;
using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::ITexture;
using shimera::Vec2;
using shimera::HDRBloomEffect;

HDRBloomEffect::HDRBloomEffect(IBackend *backend)
    : HDRBloomEffect(backend, 0.7f, 0.2f, 1.0f, 5.0f, 15, Vec2<float>(1920.0f, 1080.0f)) {}

HDRBloomEffect::HDRBloomEffect(IBackend *backend, const float threshold, const float knee, const float intensity,
                               const float blurSigma, const int blurSamples, const Vec2<float> resolution)
    : m_uThreshold(threshold), m_uKnee(knee), m_uIntensity(intensity),
      m_uBlurSigma(blurSigma), m_uBlurSamples(blurSamples), m_uResolution(resolution) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/hdr_bloom.frag"
        )
    );
    m_brightBlurredHBuffer = std::unique_ptr<IFrameBuffer>(
        backend->createFrameBuffer(static_cast<int>(resolution.x), static_cast<int>(resolution.y))
    );

    m_processor->setUniform("u_brightBlurredH", 1);
}

void HDRBloomEffect::render(ITexture &texture) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_mode", 0);
    m_brightBlurredHBuffer->bind();
    m_processor->render(texture);
    m_brightBlurredHBuffer->unbind();

    m_processor->setUniform("u_mode", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_brightBlurredHBuffer->getTexture().getNativeHandle());
    glActiveTexture(GL_TEXTURE0);
    m_processor->render(texture);
}

void HDRBloomEffect::render(ITexture &texture, IFrameBuffer &target) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_mode", 0);
    m_brightBlurredHBuffer->bind();
    m_processor->render(texture);
    m_brightBlurredHBuffer->unbind();

    m_processor->setUniform("u_mode", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_brightBlurredHBuffer->getTexture().getNativeHandle());
    glActiveTexture(GL_TEXTURE0);
    target.bind();
    m_processor->render(texture);
    target.unbind();
}

void HDRBloomEffect::updateUniforms() {
    m_processor->setUniform("u_threshold", m_uThreshold);
    m_processor->setUniform("u_knee", m_uKnee);
    m_processor->setUniform("u_intensity", m_uIntensity);
    m_processor->setUniform("u_sigma", m_uBlurSigma);
    m_processor->setUniform("u_samples", m_uBlurSamples);
    m_processor->setUniform("u_resolution", m_uResolution);
}

HDRBloomEffect &HDRBloomEffect::withThreshold(float threshold) {
    m_uThreshold = threshold;
    return *this;
}

HDRBloomEffect &HDRBloomEffect::withKnee(float knee) {
    m_uKnee = knee;
    return *this;
}

HDRBloomEffect &HDRBloomEffect::withIntensity(float intensity) {
    m_uIntensity = intensity;
    return *this;
}

HDRBloomEffect &HDRBloomEffect::withBlurSigma(float sigma) {
    m_uBlurSigma = sigma;
    return *this;
}

HDRBloomEffect &HDRBloomEffect::withBlurSamples(int samples) {
    m_uBlurSamples = samples;
    return *this;
}

HDRBloomEffect &HDRBloomEffect::withResolution(Vec2<float> resolution) {
    m_uResolution = resolution;
    m_brightBlurredHBuffer->resize(static_cast<int>(resolution.x), static_cast<int>(resolution.y));
    return *this;
}

std::string HDRBloomEffect::getName() const {
    return "HDRBloom";
}
