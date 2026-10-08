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
uniform float2 u_resolution;
uniform float2 u_direction;
uniform float u_sigma;
uniform int u_samples;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float2 texelSize = 1.0 / u_resolution;

    float centerWeight = gaussian(0.0, u_sigma);
    float4 result = u_screenTexture.Sample(input.texCoords) * centerWeight;
    float totalWeight = centerWeight;

    for (int i = 1; i <= u_samples; i++) {
        float weight = gaussian(float(i), u_sigma);
        float2 off = u_direction * texelSize * float(i);
        result += u_screenTexture.Sample(input.texCoords + off) * weight;
        result += u_screenTexture.Sample(input.texCoords - off) * weight;
        totalWeight += 2.0 * weight;
    }

    return result / totalWeight;
}