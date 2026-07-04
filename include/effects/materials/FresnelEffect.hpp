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

#ifndef SHIMERA_FRESNELEFFECT_HPP
#define SHIMERA_FRESNELEFFECT_HPP

#include "MaterialEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API FresnelEffect : public MaterialEffect<FresnelEffect> {
    public:
        explicit FresnelEffect(IBackend* backend);

        void uploadUniforms(IMaterial& material);

        FresnelEffect& withColor(Vec3<float> color);
        FresnelEffect& withPower(float power);
        FresnelEffect& withReflectance(float reflectance);
        FresnelEffect& withIntensity(float intensity);

    private:
        Vec3<float> m_color{0.3f, 0.7f, 1.0f};
        float m_power = 3.0f, m_reflectance = 0.04f, m_intensity = 1.5f;
};

}

#endif //SHIMERA_FRESNELEFFECT_HPP
