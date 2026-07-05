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
uniform vec2 u_resolution;
uniform vec2 u_direction;
uniform float u_sigma;
uniform int u_samples;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

void main() {
    vec2 texelSize = 1.0 / u_resolution;

    float centerWeight = gaussian(0.0, u_sigma);
    vec4 result = texture(u_screenTexture, texCoords) * centerWeight;
    float totalWeight = centerWeight;

    for (int i = 1; i <= u_samples; i++) {
        float weight = gaussian(float(i), u_sigma);
        vec2 off = u_direction * texelSize * float(i);
        result += texture(u_screenTexture, texCoords + off) * weight;
        result += texture(u_screenTexture, texCoords - off) * weight;
        totalWeight += 2.0 * weight;
    }

    color = result / totalWeight;
}
