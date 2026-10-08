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
#include "uniform/Vec2.inl"

namespace shimera {

class Pixelisation final : public ShaderEffect<Pixelisation> {
    public:
        explicit Pixelisation(float m_pixelSizeX = 4.0f,
                          float m_pixelSizeY = 4.0f,
                          float gap = 0.3f,
                          Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f),
                          Vec2<float> m_uOffset = Vec2(0.0f, 0.0f));

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Pixelisation"; }

        Pixelisation &withPixelSize(float pixelSize);
        Pixelisation &withPixelSizeX(float pixelSizeX);
        Pixelisation &withPixelSizeY(float pixelSizeY);
        Pixelisation &withResolution(Vec2<float> resolution);
        Pixelisation &withOffset(Vec2<float> offset);
        
        [[nodiscard]] float getPixelSize() const { return m_pixelSizeX; }
        [[nodiscard]] float getPixelSizeX() const { return m_pixelSizeX; }
        [[nodiscard]] float getPixelSizeY() const { return m_pixelSizeY; }
        [[nodiscard]] Vec2<float> getResolution() const { return m_resolution; }
        [[nodiscard]] Vec2<float> getOffset() const { return m_offset; }

    private:
        float m_pixelSizeX;
        float m_pixelSizeY;
        Vec2<float> m_resolution;
        Vec2<float> m_offset;
};

}
