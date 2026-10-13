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
#include "uniform/Vec3.inl"

namespace shimera {

// The beta's Colorshift, renamed.
class ColorTint final : public ShaderEffect<ColorTint> {
    public:
        explicit ColorTint(Vec3<float> tint = Vec3(0.0f, 0.0f, 0.0f));

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "ColorTint"; }

        ColorTint& withTint(Vec3<float> tint);

        [[nodiscard]] Vec3<float> getTint() const { return m_tint; }

    private:
        Vec3<float> m_tint;
};

}
