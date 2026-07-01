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

#ifndef SHIMERA_SFML_CHROMATICABERRATION_HPP
#define SHIMERA_SFML_CHROMATICABERRATION_HPP

#include "shimera_api.h"
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API ChromaticAberrationEffect final : public ShaderEffect<ChromaticAberrationEffect> {
    public:
        float m_uStrength = 1.0f;
        bool m_uRadius = false;
        float m_uContrast = 2.0f;
        int m_uSamples = 20;

        explicit ChromaticAberrationEffect(IBackend *backend);

        ChromaticAberrationEffect(IBackend *backend,
            float strength,
            bool radius,
            float contrast = 2.0f,
            int samples = 20
        );

        void updateUniforms() override;

        ChromaticAberrationEffect &withStrength(float s);
        ChromaticAberrationEffect &withRadius(bool r);
        ChromaticAberrationEffect &withContrast(float c);
        ChromaticAberrationEffect &withSamples(int s);
};

}

#endif //SHIMERA_SFML_CHROMATICABERRATION_HPP