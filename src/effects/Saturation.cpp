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

#include "Saturation.hpp"

#include "Resources.inl"

using shimera::Saturation;

Saturation::Saturation(const float saturationFactor)
    : m_saturationFactor(saturationFactor) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("saturation.frag"));
}

void Saturation::updateUniforms() {
    m_processor->setUniform("u_saturationFactor", m_saturationFactor);
}

Saturation& Saturation::withSaturationFactor(const float saturationFactor) {
    m_saturationFactor = saturationFactor;
    return *this;
}
