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
#include "uniform/Vec4.inl"

namespace shimera {

class Vignette final : public ShaderEffect<Vignette> {
    public:
        explicit Vignette(float strength = 1.0f,
                          float radius = 0.5f,
                          float gap = 0.3f,
                          Vec4<float> color = Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                          bool rounded = false);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Vignette"; }

        Vignette& withStrength(float strength);
        Vignette& withRadius(float radius);
        Vignette& withGap(float gap);
        Vignette& withColor(Vec4<float> color);
        Vignette& withRounded(bool rounded);

        [[nodiscard]] float getStrength() const { return m_strength; }

    private:
        float m_strength;
        float m_radius;
        float m_gap;
        Vec4<float> m_color;
        int m_rounded;
};

}
