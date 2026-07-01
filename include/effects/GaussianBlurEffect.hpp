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

#ifndef SHIMERA_GAUSSIANBLUREFFECT_HPP
#define SHIMERA_GAUSSIANBLUREFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "backend/IFrameBuffer.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API GaussianBlurEffect final : public ShaderEffect<GaussianBlurEffect> {
    public:
        float m_uSigma = 3.0f;
        int m_uSamples = 10;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);

        explicit GaussianBlurEffect(IBackend *backend);

        GaussianBlurEffect(IBackend *backend,
            float sigma,
            int samples,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(ITexture &texture) override;
        void render(ITexture &texture, IFrameBuffer &target) override;
        void updateUniforms() override;

        GaussianBlurEffect &withSigma(float sigma);
        GaussianBlurEffect &withSamples(int samples);
        GaussianBlurEffect &withResolution(Vec2<float> resolution);

    private:
        std::unique_ptr<IFrameBuffer> m_intermediateBuffer;
};

}

#endif //SHIMERA_GAUSSIANBLUREFFECT_HPP
