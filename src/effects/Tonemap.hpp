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

/**
 * Brings an HDR image back into the display range. Exposure, a tone curve, then an
 * optional gamma encode for 8-bit targets.
 *
 * EffectPipeline appends one automatically when HDR is enabled, so most users never
 * create it. It is public so a chain can place it explicitly, or tune the automatic one
 * through EffectPipeline::tonemap().
 */
class Tonemap final : public ShaderEffect<Tonemap> {
    public:
        enum class Operator { None = 0, Reinhard = 1, Aces = 2 };

        explicit Tonemap(float exposure = 1.0f,
                         Operator op = Operator::Aces,
                         bool gammaEncode = true);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Tonemap"; }

        Tonemap& withExposure(float exposure);
        Tonemap& withOperator(Operator op);
        /* Off when the scene is already display-encoded (the usual case for a host that
         * draws 8-bit sprites), on when everything upstream is linear radiance. */
        Tonemap& withGammaEncode(bool enabled);

        [[nodiscard]] float getExposure() const { return m_exposure; }
        [[nodiscard]] Operator getOperator() const { return m_operator; }
        [[nodiscard]] bool isGammaEncoded() const { return m_gammaEncode; }

    private:
        float m_exposure;
        Operator m_operator;
        bool m_gammaEncode;
};

}
