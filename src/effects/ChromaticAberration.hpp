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

#pragma once
#include "ShaderEffect.inl"

namespace shimera {

class ChromaticAberration : public ShaderEffect<ChromaticAberration> {
    public:
        explicit ChromaticAberration(float strength = 1.0f,
            bool radius = false,
            float contrast = 2.0f,
            int samples = 20);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "ChromaticAberration"; }

        ChromaticAberration &withStrength(float s);
        ChromaticAberration &withRadius(bool r);
        ChromaticAberration &withContrast(float c);
        ChromaticAberration &withSamples(int s);

        [[nodiscard]] float getStrength() const { return m_uStrength; }
        [[nodiscard]] bool getRadius() const { return m_uRadius; }
        [[nodiscard]] float getContrast() const { return m_uContrast; }
        [[nodiscard]] int getSamples() const { return m_uSamples; }

    private:
        float m_uStrength = 1.0f;
        bool m_uRadius = false;
        float m_uContrast = 2.0f;
        int m_uSamples = 20;
};

}
