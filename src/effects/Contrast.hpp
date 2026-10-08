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

class Contrast final : public ShaderEffect<Contrast> {
    public:
        explicit Contrast(float contrastFactor = 1.2f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Contrast"; }

        Contrast& withContrastFactor(float contrastFactor);

        [[nodiscard]] float getContrastFactor() const { return m_contrastFactor; }

    private:
        float m_contrastFactor;
};

}
