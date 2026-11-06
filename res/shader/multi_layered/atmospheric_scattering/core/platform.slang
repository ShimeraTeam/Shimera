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

/*
 * Everything that differs between graphics APIs lives here, and nowhere else.
 */
module platform;

public static const int kDepthRangeNegOneToOne = 0; // OpenGL default clip space
public static const int kDepthRangeZeroToOne = 1; // Vulkan, Metal, Direct3D

public struct DepthConvention {
    public int range; // kDepthRange*
    public int reversed; // 1 when the near plane is stored as 1 and the far plane as 0
    public int flipY; // 1 when texture v grows downwards relative to clip y (Vulkan)
    public int linearInput; // 1 when the depth texture already holds view distance
};

// Hardware depth as stored in the texture, the z the inverse projection expects.
public float rawDepthToNdc(float raw, DepthConvention c) {
    return c.range == kDepthRangeNegOneToOne ? raw * 2.0 - 1.0 : raw;
}

// True for pixels nothing was drawn on. The epsilon absorbs 24-bit depth quantisation.
public bool isFarPlane(float raw, DepthConvention c) {
    const float eps = 1e-6;
    return c.reversed != 0 ? raw <= eps : raw >= 1.0 - eps;
}

public float2 uvToNdcXY(float2 uv, DepthConvention c) {
    float2 ndc = uv * 2.0 - 1.0;
    if (c.flipY != 0)
        ndc.y = -ndc.y;
    return ndc;
}

// View-space position of the pixel at depth `raw`. The camera sits at the origin.
public float3 viewPositionFromDepth(float2 uv, float raw, float4x4 invProjection, DepthConvention c) {
    const float4 clip = float4(uvToNdcXY(uv, c), rawDepthToNdc(raw, c), 1.0);
    const float4 view = mul(invProjection, clip);
    return view.xyz / view.w;
}

/* Unit direction, in view space, of the ray through the pixel. Any depth gives a point on
 * that ray since the camera is the view-space origin. The middle of the range keeps w away
 * from zero with infinite far planes. */
public float3 viewRayDirection(float2 uv, float4x4 invProjection, DepthConvention c) {
    const float z = c.range == kDepthRangeNegOneToOne ? 0.0 : 0.5;
    const float4 clip = float4(uvToNdcXY(uv, c), z, 1.0);
    const float4 view = mul(invProjection, clip);
    return normalize(view.xyz / view.w);
}
