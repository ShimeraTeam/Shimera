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

#pragma once

#include "ShaderEffect.inl"

namespace shimera {

class GaussianBlur final : public ShaderEffect<GaussianBlur> {
    public:
        explicit GaussianBlur(float sigma = 3.0f, int samples = 10, Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(const GLTexture& input) override;
        void render(const GLTexture& input, GLFramebuffer& target) override;
        void resize(int width, int height) override;
        void updateUniforms() override;

        [[nodiscard]] std::string getName() const override { return "GaussianBlur"; }

        GaussianBlur& withSigma(float sigma);
        GaussianBlur& withSamples(int samples);
        GaussianBlur& withResolution(Vec2<float> resolution);

        [[nodiscard]]float getSigma() const { return m_sigma; }
        [[nodiscard]]int getSamples() const { return m_samples; }
        [[nodiscard]]Vec2<float> getResolution() const { return m_uResolution; }

    private:
        void renderImpl(const GLTexture& input, uint32_t target);
    
        float m_sigma;
        int m_samples;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);
        std::unique_ptr<GLFramebuffer> m_intermediateBuffer;
};

}