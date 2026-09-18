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

#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform sampler2D u_brightBlurredH;
uniform int u_mode;
uniform float u_threshold;
uniform float u_knee;
uniform float u_intensity;
uniform float u_sigma;
uniform int u_samples;
uniform vec2 u_resolution;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

vec3 brightPass(vec3 c) {
    float brightness = max(c.r, max(c.g, c.b));
    float w = smoothstep(u_threshold - u_knee, u_threshold + u_knee, brightness);
    return c * w;
}

void main() {
    vec2 texelSize = 1.0 / u_resolution;

    if (u_mode == 0) {
        float centerWeight = gaussian(0.0, u_sigma);
        vec3 result = brightPass(texture(u_screenTexture, texCoords).rgb) * centerWeight;
        float totalWeight = centerWeight;

        for (int i = 1; i <= u_samples; i++) {
            float w = gaussian(float(i), u_sigma);
            vec2 off = vec2(texelSize.x * float(i), 0.0);
            result += brightPass(texture(u_screenTexture, texCoords + off).rgb) * w;
            result += brightPass(texture(u_screenTexture, texCoords - off).rgb) * w;
            totalWeight += 2.0 * w;
        }

        color = vec4(result / totalWeight, 1.0);
    } else {
        float centerWeight = gaussian(0.0, u_sigma);
        vec3 bloom = texture(u_brightBlurredH, texCoords).rgb * centerWeight;
        float totalWeight = centerWeight;

        for (int i = 1; i <= u_samples; i++) {
            float w = gaussian(float(i), u_sigma);
            vec2 off = vec2(0.0, texelSize.y * float(i));
            bloom += texture(u_brightBlurredH, texCoords + off).rgb * w;
            bloom += texture(u_brightBlurredH, texCoords - off).rgb * w;
            totalWeight += 2.0 * w;
        }
        bloom /= totalWeight;

        vec3 scene = texture(u_screenTexture, texCoords).rgb;
        color = vec4(scene + bloom * u_intensity, 1.0);
    }
}
