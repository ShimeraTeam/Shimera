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

#include "postprocess.slang"

uniform Sampler2D u_screenTexture;

uniform float u_strength;
uniform float u_radius;
uniform float u_gap;
uniform float4 u_color;
uniform int u_isRounded;
uniform float2 u_resolution;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float4 texColor = u_screenTexture.Sample(input.texCoords);

    float dist;
    if (u_isRounded == 1) {
        const float2 aspect = float2(u_resolution.x / u_resolution.y, 1.0);
        dist = distance(input.texCoords * aspect, float2(0.5, 0.5) * aspect);
    } else {
        dist = distance(input.texCoords, float2(0.5, 0.5));
    }

    const float vignette = smoothstep(u_radius, u_radius + u_gap, dist) * u_strength;
    return float4(lerp(texColor.rgb, u_color.rgb, vignette), texColor.a);
}
