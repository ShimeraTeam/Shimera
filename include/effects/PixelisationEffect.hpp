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

#ifndef SHIMERA_PIXELISATIONEFFECT_HPP
#define SHIMERA_PIXELISATIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API PixelisationEffect final : public ShaderEffect<PixelisationEffect> {
    public:
        float m_uPixelSizeX = 4.0f;
        float m_uPixelSizeY = 4.0f;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);
        Vec2<float> m_uOffset = Vec2(0.0f, 0.0f);

        explicit PixelisationEffect(IBackend *backend);

        PixelisationEffect(IBackend *backend,
            float pixelSize,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void updateUniforms() override;

        PixelisationEffect &withPixelSize(float pixelSize);
        PixelisationEffect &withPixelSizeX(float pixelSizeX);
        PixelisationEffect &withPixelSizeY(float pixelSizeY);
        PixelisationEffect &withResolution(Vec2<float> resolution);
        PixelisationEffect &withOffset(Vec2<float> offset);

        std::string getName() const override;
};

}

#endif //SHIMERA_PIXELISATIONEFFECT_HPP
