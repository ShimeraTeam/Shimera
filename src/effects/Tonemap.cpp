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

#include "Tonemap.hpp"

#include "Resources.inl"

using shimera::Tonemap;

Tonemap::Tonemap(const float exposure, const Operator op, const bool gammaEncode)
    : m_exposure(exposure), m_operator(op), m_gammaEncode(gammaEncode) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("tonemap.frag"));
}

void Tonemap::updateUniforms() {
    m_processor->setUniform("u_exposure", m_exposure);
    m_processor->setUniform("u_operator", static_cast<int>(m_operator));
    m_processor->setUniform("u_gammaEncode", m_gammaEncode ? 1 : 0);
}

Tonemap& Tonemap::withExposure(const float exposure) {
    m_exposure = exposure;
    return *this;
}

Tonemap& Tonemap::withOperator(const Operator op) {
    m_operator = op;
    return *this;
}

Tonemap& Tonemap::withGammaEncode(const bool enabled) {
    m_gammaEncode = enabled;
    return *this;
}
