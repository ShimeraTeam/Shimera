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
 * Reading the lookup tables. These are the only functions that sample a LUT, so the
 * layouts in lut_mapping.slang stay an internal detail.
 */
module lookups;

import params;
import geometry;
import lut_mapping;

// Transmittance from (r, mu) to the top of the atmosphere.
public float3 sampleTransmittance(Sampler2D lut, AtmosphereParams p, float r, float mu) {
    const float rc = clamp(r, p.groundRadius, p.topRadius);
    const float2 uv = transmittanceUv(rc, mu, p.groundRadius, p.topRadius, p.transmittanceLutSize.xy);
    return lut.Sample(uv).rgb;
}

// Multiple-scattering contribution per unit scattering coefficient, for a unit sun.
public float3 sampleMultiScatter(Sampler2D lut, AtmosphereParams p, float r, float sunCos) {
    const float2 uv = multiScatterUv(r, sunCos, p.groundRadius, p.topRadius, p.multiScatterLutSize.xy);
    return lut.Sample(uv).rgb;
}

/* The local frame the sky-view LUT is built in. Up at the camera, the sun's horizontal
 * direction as azimuth zero, and the perpendicular to complete it. */
public void skyViewFrame(AtmosphereParams p, out float3 up, out float3 forward, out float3 right) {
    up = cameraUp(p);
    const float3 sun = p.sunDirection.xyz;
    float3 horizontal = sun - up * dot(sun, up);
    if (dot(horizontal, horizontal) < 1e-8) {
        horizontal = abs(up.y) < 0.99 ? cross(up, float3(0.0, 1.0, 0.0)) : cross(up, float3(1.0, 0.0, 0.0));
    }
    forward = normalize(horizontal);
    right = cross(up, forward);
}

// Direction for a (zenith cosine, azimuth) pair in the sky-view frame.
public float3 skyViewDirection(float viewZenithCos, float azimuth, float3 up, float3 forward, float3 right) {
    const float sinZenith = safeSqrt(1.0 - viewZenithCos * viewZenithCos);
    return viewZenithCos * up + sinZenith * (cos(azimuth) * forward + sin(azimuth) * right);
}

// Sky luminance in direction `dir` from the camera, from the sky-view LUT.
public float3 sampleSkyView(Sampler2D lut, AtmosphereParams p, float3 dir) {
    float3 up, forward, right;
    skyViewFrame(p, up, forward, right);
    const float r = clampedRadius(p, length(p.cameraPos.xyz));
    const float viewZenithCos = dot(dir, up);
    const float azimuth = atan2(dot(dir, right), dot(dir, forward));
    const bool hitsGround = intersectsGround(r, viewZenithCos, p.groundRadius);
    const float2 uv = skyViewUv(r, viewZenithCos, azimuth, hitsGround, p.groundRadius, p.skyViewLutSize.xy);
    return lut.Sample(uv).rgb;
}

/* Aerial perspective for an opaque pixel at `distance` km. rgb is the in-scattered
 * luminance, a the (mean) transmittance. */
public float4 sampleAerial(Sampler2D atlas, AtmosphereParams p, float2 screenUv, float distance) {
    const float slices = p.aerial.x;
    const float tile = p.aerial.y;
    const float maxDistance = p.aerial.z;

    const float w = aerialDistanceToSlice(distance, slices, maxDistance);
    float t = w - 1.0;
    float weight = 1.0;
    if (t < 0.0) {
        weight = saturate(w);
        t = 0.0;
    }
    const int last = int(slices) - 1;
    const int i0 = min(int(floor(t)), last);
    const int i1 = min(i0 + 1, last);
    const float f = saturate(t - float(i0));

    const float4 a = atlas.Sample(aerialAtlasUv(screenUv, i0, slices, tile));
    const float4 b = atlas.Sample(aerialAtlasUv(screenUv, i1, slices, tile));
    float4 result = lerp(a, b, f);
    result.rgb *= weight;
    result.a = lerp(1.0, result.a, weight);
    return result;
}
