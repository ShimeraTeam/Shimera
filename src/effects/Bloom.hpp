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

class Bloom final : public ShaderEffect<Bloom> {
    public:
        explicit Bloom(float threshold = 0.7f,
                          float knee = 0.2f,
                          float intensity = 1.0f,
                          float blurSigma = 5.0f,
                          int blurSamples = 15);

        void render(const GLTexture& input) override;
        void render(const GLTexture& input, GLFramebuffer& target) override;
        void updateUniforms() override;
        void resize(int width, int height) override;

        [[nodiscard]] std::string getName() const override { return "Bloom"; }

        Bloom& withThreshold(float threshold);
        Bloom& withKnee(float knee);
        Bloom& withIntensity(float intensity);
        Bloom& withBlurSigma(float sigma);
        Bloom& withBlurSamples(int samples);

        [[nodiscard]] float getThreshold() const { return m_threshold; }
        [[nodiscard]] float getKnee() const { return m_knee; }
        [[nodiscard]] float getIntensity() const { return m_intensity; }
        [[nodiscard]] float getBlurSigma() const { return m_blurSigma; }
        [[nodiscard]] int getBlurSamples() const { return m_blurSamples; }

    private:
        void renderImpl(const GLTexture& input, uint32_t target);

        std::unique_ptr<GLFramebuffer> m_brightBlurredH;
        float m_threshold;
        float m_knee;
        float m_intensity;
        float m_blurSigma;
        int m_blurSamples;
};

}
