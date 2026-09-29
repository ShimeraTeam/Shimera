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

#include "postprocess.slang"

uniform Sampler2D u_screenTexture;

uniform float u_strength;
uniform int u_radius;
uniform float u_contrast;
uniform int u_samples;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float4 colorSum = float4(0.0);
    float4 weightSum = float4(0.0);

    // Optimization: Set the direction here instead of inside the loop
    float2 direction = (u_radius >= 1) ? (0.5 - input.texCoords) * 0.1 : float2(0.04);
    direction *= u_strength;

    int samples = max(u_samples, 1);
    float step = 1.0 / samples;
    for (int s = 0; s <= samples; ++s) {
        float i = s * step;

        float4 weight = float4(i, 1.0 - abs(i * 2 - 1.0), 1.0 - i, 0.5);
        weight = 0.5 + (weight - 0.5) * u_contrast;

        float4 col = u_screenTexture.Sample(input.texCoords + (i - 0.5) * direction);
        colorSum += col * col * weight;

        weightSum += weight;
    }
    return sqrt(clamp(colorSum / weightSum, 0.0, 1.0));
}
