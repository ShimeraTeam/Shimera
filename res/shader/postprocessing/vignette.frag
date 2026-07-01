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
uniform float u_strength;
uniform float u_radius;
uniform float u_gap;
uniform vec4 u_color;
uniform int u_isRounded;
uniform vec2 u_resolution;

void main() {
    vec4 texColor = texture(u_screenTexture, texCoords);
    float dist = 0.0;
    if (u_isRounded == 1) {
        vec2 aspect = vec2(u_resolution.x / u_resolution.y, 1.0);
        dist = distance(texCoords * aspect, vec2(0.5) * aspect);
    } else {
        dist = distance(texCoords, vec2(0.5));
    }
    float vignette = smoothstep(u_radius, u_radius + u_gap, dist);
    vignette = vignette * u_strength;
    color = vec4(mix(texColor.rgb, u_color.rgb, vignette), texColor.a);
}
