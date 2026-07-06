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

#include "effects/materials/FresnelEffect.hpp"

using shimera::FresnelEffect;
using shimera::IBackend;
using shimera::IMaterial;
using shimera::Vec3;

FresnelEffect::FresnelEffect(IBackend* backend)
    : MaterialEffect(backend,
        "../../../../res/shader/material/fresnel.vert",
        "../../../../res/shader/material/fresnel.frag") {}

void FresnelEffect::uploadUniforms(IMaterial& material) {
    material.setUniform("u_color", m_color);
    material.setUniform("u_power", m_power);
    material.setUniform("u_reflectance", m_reflectance);
    material.setUniform("u_intensity", m_intensity);
}

FresnelEffect& FresnelEffect::withColor(Vec3<float> c) {
    m_color = c;
    return *this;
}
FresnelEffect& FresnelEffect::withPower(float p) {
    m_power = p;
    return *this;
}
FresnelEffect& FresnelEffect::withReflectance(float r) {
    m_reflectance = r;
    return *this;
}
FresnelEffect& FresnelEffect::withIntensity(float i) {
    m_intensity = i;
    return *this;
}


