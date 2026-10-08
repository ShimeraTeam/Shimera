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
uniform Sampler2D u_brightBlurredH;

uniform int u_mode;
uniform float u_threshold;
uniform float u_knee;
uniform float u_intensity;
uniform float u_sigma;
uniform int u_samples;
uniform float2 u_resolution;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

float3 brightPass(float3 c) {
    const float brightness = max(c.r, max(c.g, c.b));
    const float w = smoothstep(u_threshold - u_knee, u_threshold + u_knee, brightness);
    return c * w;
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float2 texelSize = 1.0 / u_resolution;
    const float centerWeight = gaussian(0.0, u_sigma);
    float totalWeight = centerWeight;

    if (u_mode == 0) {
        float3 result = brightPass(u_screenTexture.Sample(input.texCoords).rgb) * centerWeight;

        for (int i = 1; i <= u_samples; i++) {
            const float w = gaussian(float(i), u_sigma);
            const float2 off = float2(texelSize.x * float(i), 0.0);
            result += brightPass(u_screenTexture.Sample(input.texCoords + off).rgb) * w;
            result += brightPass(u_screenTexture.Sample(input.texCoords - off).rgb) * w;
            totalWeight += 2.0 * w;
        }

        return float4(result / totalWeight, 1.0);
    }

    float3 bloom = u_brightBlurredH.Sample(input.texCoords).rgb * centerWeight;

    for (int i = 1; i <= u_samples; i++) {
        const float w = gaussian(float(i), u_sigma);
        const float2 off = float2(0.0, texelSize.y * float(i));
        bloom += u_brightBlurredH.Sample(input.texCoords + off).rgb * w;
        bloom += u_brightBlurredH.Sample(input.texCoords - off).rgb * w;
        totalWeight += 2.0 * w;
    }
    bloom /= totalWeight;

    const float3 scene = u_screenTexture.Sample(input.texCoords).rgb;
    return float4(scene + bloom * u_intensity, 1.0);
}
