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

class Distortion final : public ShaderEffect<Distortion> {
    public:
        explicit Distortion(float noiseScale = 3.0f,
                            float distortionStrength = 0.13f,
                            float timeScale = 0.1f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Distortion"; }

        Distortion& withTime(float time);
        Distortion& withNoiseScale(float noiseScale);
        Distortion& withDistortionStrength(float distortionStrength);
        Distortion& withTimeScale(float timeScale);
        
        [[nodiscard]] float getTime() const { return m_time; }
        [[nodiscard]] float getNoiseScale() const { return m_noiseScale; }
        [[nodiscard]] float getDistortionStrength() const { return m_distortionStrength; }
        [[nodiscard]] float getTimeScale() const { return m_timeScale; }

    private:
        float m_time = 0.0f;
        float m_noiseScale;
        float m_distortionStrength;
        float m_timeScale;
};

}
