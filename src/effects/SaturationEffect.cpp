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

#include "effects/SaturationEffect.hpp"

using shimera::IBackend;
using shimera::IPostProcessor;
using shimera::SaturationEffect;

SaturationEffect::SaturationEffect(IBackend *backend)
    : SaturationEffect(backend, 1.0f) {}

SaturationEffect::SaturationEffect(IBackend *backend, float u_strength)
    : m_uStrength(u_strength) {
     m_processor = std::unique_ptr<IPostProcessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/saturation.frag"
            )
        );
}

void SaturationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
}

SaturationEffect &SaturationEffect::withStrength(float u_strength) {
    this->m_uStrength = u_strength;
    return *this;
}