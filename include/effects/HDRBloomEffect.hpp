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

#ifndef SHIMERA_HDRBLOOMEFFECT_HPP
#define SHIMERA_HDRBLOOMEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "backend/IFrameBuffer.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API HDRBloomEffect final : public ShaderEffect<HDRBloomEffect> {
    public:
        float m_uThreshold = 0.7f;
        float m_uKnee = 0.2f;
        float m_uIntensity = 1.0f;
        float m_uBlurSigma = 5.0f;
        int m_uBlurSamples = 15;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);

        explicit HDRBloomEffect(IBackend *backend);

        HDRBloomEffect(IBackend *backend,
            float threshold,
            float knee,
            float intensity,
            float blurSigma,
            int blurSamples,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(ITexture &texture) override;
        void render(ITexture &texture, IFrameBuffer &target) override;
        void updateUniforms() override;

        HDRBloomEffect &withThreshold(float threshold);
        HDRBloomEffect &withKnee(float knee);
        HDRBloomEffect &withIntensity(float intensity);
        HDRBloomEffect &withBlurSigma(float sigma);
        HDRBloomEffect &withBlurSamples(int samples);
        HDRBloomEffect &withResolution(Vec2<float> resolution);

        std::string getName() const override;

    private:
        std::unique_ptr<IFrameBuffer> m_brightBlurredHBuffer;
};

}

#endif //SHIMERA_HDRBLOOMEFFECT_HPP
