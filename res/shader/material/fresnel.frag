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
in vec3 vWorldPos;
in vec3 vNormal;
out vec4 color;

uniform vec3  u_cameraPos;
uniform vec3  u_color;
uniform float u_power;
uniform float u_reflectance;
uniform float u_intensity;

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(u_cameraPos - vWorldPos);
    float fresnel = u_reflectance + (1.0 - u_reflectance) * pow(1.0 - max(dot(N, V), 0.0), u_power);
    fresnel *= u_intensity;
    color = vec4(u_color * fresnel, fresnel);
}