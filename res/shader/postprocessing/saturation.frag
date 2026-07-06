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

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float u_strength;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);

    // ITU-R BT.709 luma coefficients (HD standard)
    float luminance = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    vec3 saturated = mix(vec3(luminance), texColor.rgb, u_strength);

    color = vec4(saturated, texColor.a);
}
