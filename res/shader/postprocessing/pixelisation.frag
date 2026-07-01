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
uniform float u_pixelSizeX;
uniform float u_pixelSizeY;
uniform vec2 u_resolution;
uniform vec2 u_offset;

void main() {
    vec2 pixelUV = vec2(u_pixelSizeX, u_pixelSizeY) / u_resolution;
    vec2 coord = floor((texCoords - u_offset) / pixelUV) * pixelUV + u_offset + pixelUV * 0.5;
    color = texture(u_screenTexture, coord);
}
