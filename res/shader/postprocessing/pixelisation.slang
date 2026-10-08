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
uniform float u_pixelSizeX;
uniform float u_pixelSizeY;
uniform float2 u_resolution;
uniform float2 u_offset;

[shader("fragment")]
void fragMain(VertexOutput input, out float4 color) : SV_Target{
    float2 pixelUV = float2(u_pixelSizeX, u_pixelSizeY) / u_resolution;
    float2 coord = floor((input.texCoords - u_offset) / pixelUV) * pixelUV + u_offset + pixelUV * 0.5;
    color = u_screenTexture.Sample(coord);
}
