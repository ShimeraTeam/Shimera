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

namespace shimera {

class Brightness final : public ShaderEffect<Brightness> {
    public:
        explicit Brightness(float strength = 0.1f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Brightness"; }

        Brightness& withStrength(float strength);

        [[nodiscard]] float getStrength() const { return m_strength; }

    private:
        float m_strength;
};

}
