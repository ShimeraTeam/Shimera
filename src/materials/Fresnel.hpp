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

#include "ShaderMaterial.inl"
#include "uniform/Vec3.inl"

namespace shimera {

/**
 * Fresnel, bright where the surface turns away from the camera.
 */
class Fresnel final : public ShaderMaterial<Fresnel> {
    public:
        explicit Fresnel(Vec3<float> color = Vec3(0.3f, 0.6f, 1.0f),
                         float power = 3.0f,
                         float reflectance = 0.04f,
                         float intensity = 1.5f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Fresnel"; }

        Fresnel& withColor(Vec3<float> color);
        Fresnel& withPower(float power);
        Fresnel& withReflectance(float reflectance);
        Fresnel& withIntensity(float intensity);

    private:
        Vec3<float> m_color;
        float m_power;
        float m_reflectance;
        float m_intensity;
};

}
