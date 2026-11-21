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
 * The two parameter structs every pass declares as uniforms. The C++ side fills them by
 * member name ("u_atm.components[1].scattering").
 *
 * The units are kilometres, 1/km, and planet-local coordinates (planet centre at the origin).
 * The host world never reaches the shaders, the C++ side converts in double precision.
 */
module params;

__exported import medium;
import platform;

public struct AtmosphereParams {
    public float groundRadius; // km
    public float topRadius; // km
    public float multiScatteringFactor; // 1 = physical, 0 = single scattering only
    public float sunAngularRadius; // radians

    public float4 groundAlbedo; // rgb
    public float4 sunIlluminance; // rgb
    public float4 sunDirection; // xyz
    public float4 cameraPos; // xyz

    public MediumComponent components[4];

    public float4 transmittanceLutSize; // xy = texels, zw = 1 / texels
    public float4 multiScatterLutSize;
    public float4 skyViewLutSize;
    public float4 aerial; // x slices, y tile texels, z max distance km, w unused
    public float4 sunDisk; // x intensity scale, y limb darkening, zw unused
};

public struct ViewParams {
    public float4x4 invProjection; // clip -> view
    public float4x4 viewToPlanet; // rotation only, view-space direction -> planet-local
    public int4 depth; // DepthConvention, range, reversed, flipY, linearInput
    public float4 scale; // x = km per host world unit, y = aerial distance scale, z = 1 in sky mode, w unused
};

/* In sky mode the planet is only there for the lookups, so rays towards the host's geometry must not stop where
 * they would meet the invisible planet's curved surface. */
public bool isFlatWorld(ViewParams v) {
    return v.scale.z > 0.5;
}

public DepthConvention depthConvention(ViewParams v) {
    DepthConvention c;
    c.range = v.depth.x;
    c.reversed = v.depth.y;
    c.flipY = v.depth.z;
    c.linearInput = v.depth.w;
    return c;
}

// Radius used for LUT lookups at the camera.
public float clampedRadius(AtmosphereParams p, float r) {
    const float margin = (p.topRadius - p.groundRadius) * 1e-4;
    return clamp(r, p.groundRadius + margin, p.topRadius - margin);
}

// Local up at the camera, a camera sitting exactly on the centre gets an arbitrary up.
public float3 cameraUp(AtmosphereParams p) {
    const float len = length(p.cameraPos.xyz);
    return len > 1e-6 ? p.cameraPos.xyz / len : float3(0.0, 1.0, 0.0);
}
