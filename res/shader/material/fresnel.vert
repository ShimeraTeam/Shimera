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
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal; // location to be changed in the future

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 vWorldPos;
out vec3 vNormal;

void main() {
    vec4 world = u_model * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(u_model))) * aNormal;
    gl_Position = u_projection * u_view * world;
}