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

#include "effects/GaussianBlurEffect.hpp"

using shimera::IBackend;
using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::ITexture;
using shimera::Vec2;
using shimera::GaussianBlurEffect;

GaussianBlurEffect::GaussianBlurEffect(IBackend *backend)
    : GaussianBlurEffect(backend, 3.0f, 10, Vec2<float>(1920.0f, 1080.0f)) {}

GaussianBlurEffect::GaussianBlurEffect(IBackend *backend, const float sigma, const int samples, const Vec2<float> resolution)
    : m_uSigma(sigma), m_uSamples(samples), m_uResolution(resolution) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/gaussian_blur.frag"
        )
    );
    m_intermediateBuffer = std::unique_ptr<IFrameBuffer>(
        backend->createFrameBuffer(static_cast<int>(resolution.x), static_cast<int>(resolution.y))
    );
}

void GaussianBlurEffect::render(ITexture &texture) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_direction", Vec2(1.0f, 0.0f));
    m_intermediateBuffer->bind();
    m_processor->render(texture);
    m_intermediateBuffer->unbind();

    m_processor->setUniform("u_direction", Vec2(0.0f, 1.0f));
    m_processor->render(m_intermediateBuffer->getTexture());
}

void GaussianBlurEffect::render(ITexture &texture, IFrameBuffer &target) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_direction", Vec2(1.0f, 0.0f));
    m_intermediateBuffer->bind();
    m_processor->render(texture);
    m_intermediateBuffer->unbind();

    m_processor->setUniform("u_direction", Vec2(0.0f, 1.0f));
    target.bind();
    m_processor->render(m_intermediateBuffer->getTexture());
    target.unbind();
}

void GaussianBlurEffect::updateUniforms() {
    m_processor->setUniform("u_sigma", m_uSigma);
    m_processor->setUniform("u_samples", m_uSamples);
    m_processor->setUniform("u_resolution", m_uResolution);
}

GaussianBlurEffect &GaussianBlurEffect::withSigma(float sigma) {
    m_uSigma = sigma;
    return *this;
}

GaussianBlurEffect &GaussianBlurEffect::withSamples(int samples) {
    m_uSamples = samples;
    return *this;
}

GaussianBlurEffect &GaussianBlurEffect::withResolution(Vec2<float> resolution) {
    m_uResolution = resolution;
    m_intermediateBuffer->resize(static_cast<int>(resolution.x), static_cast<int>(resolution.y));
    return *this;
}
