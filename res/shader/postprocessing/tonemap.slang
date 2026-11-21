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

uniform float u_exposure;
uniform int u_operator; // 0 none (clamp), 1 Reinhard, 2 ACES filmic fit
uniform int u_gammaEncode; // 1 = apply the 1/2.2 display curve for an 8-bit target

/* Rational fit of the ACES filmic reference rendering transform, as published by
 * Krzysztof Narkowicz, "ACES Filmic Tone Mapping Curve" (2016). Five constants, no LUT. */
float3 acesFilmic(float3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

float3 reinhard(float3 x) {
    return x / (1.0 + x);
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float4 scene = u_screenTexture.Sample(input.texCoords);
    float3 color = max(scene.rgb, 0.0) * u_exposure;

    if (u_operator == 1)
        color = reinhard(color);
    else if (u_operator == 2)
        color = acesFilmic(color);
    else
        color = saturate(color);

    if (u_gammaEncode == 1)
        color = pow(color, 1.0 / 2.2);

    return float4(color, scene.a);
}
