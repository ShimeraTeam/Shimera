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
 * uv <-> physical parameters for every lookup table, in both directions.
 *
 * The pass that writes a texel and the code that reads it must agree exactly, so both
 * sides call into this file and nothing else.
 */
module lut_mapping;

import geometry;

public float2 unitToSubUv(float2 u, float2 size) {
    return 0.5 / size + u * (1.0 - 1.0 / size);
}

public float2 subUvToUnit(float2 uv, float2 size) {
    return (uv - 0.5 / size) / (1.0 - 1.0 / size);
}

// Mapping from Bruneton 2017. x follows the distance to the top boundary, which spreads
// the fast-changing region near the horizon over many texels.

public float2 transmittanceUv(float r, float mu, float ground, float top, float2 size) {
    const float H = safeSqrt(top * top - ground * ground);
    const float rho = safeSqrt(r * r - ground * ground);
    const float d = distanceToTop(r, mu, top);
    const float dMin = top - r;
    const float dMax = rho + H;
    const float xMu = (d - dMin) / max(dMax - dMin, 1e-6);
    const float xR = rho / H;
    return unitToSubUv(saturate(float2(xMu, xR)), size);
}

public void transmittanceParams(float2 uv, float ground, float top, float2 size, out float r, out float mu) {
    const float2 x = saturate(subUvToUnit(uv, size));
    const float H = safeSqrt(top * top - ground * ground);
    const float rho = H * x.y;
    r = sqrt(rho * rho + ground * ground);
    const float dMin = top - r;
    const float dMax = rho + H;
    const float d = dMin + x.x * (dMax - dMin);
    mu = d <= 0.0 ? 1.0 : (H * H - rho * rho - d * d) / (2.0 * r * d);
    mu = clamp(mu, -1.0, 1.0);
}

// Multiple-scattering LUT.

public float2 multiScatterUv(float r, float sunCos, float ground, float top, float2 size) {
    const float2 u = float2(sunCos * 0.5 + 0.5, (r - ground) / (top - ground));
    return unitToSubUv(saturate(u), size);
}

public void multiScatterParams(float2 uv, float ground, float top, float2 size, out float r, out float sunCos) {
    const float2 x = saturate(subUvToUnit(uv, size));
    sunCos = x.x * 2.0 - 1.0;
    r = ground + x.y * (top - ground);
}

// Sky-view LUT, directions around the camera
// u is the azimuth relative to the sun. v is non-linear in the view zenith angle, as in
// Hillaire 2020: the upper half covers zenith -> horizon and the lower half horizon ->
// nadir, each with a square-root curve so texels cluster at the horizon, where the sky
// changes fastest. The horizon itself moves with altitude.

public float2 skyViewUv(float r, float viewZenithCos, float azimuth, bool hitsGround, float ground, float2 size) {
    const float vHorizon = safeSqrt(r * r - ground * ground);
    const float beta = safeAcos(vHorizon / r); // angle between horizon and nadir
    const float zenithHorizon = kPi - beta; // zenith angle of the horizon
    const float viewZenith = safeAcos(viewZenithCos);

    float v;
    if (!hitsGround) {
        const float c = viewZenith / max(zenithHorizon, 1e-6);
        v = (1.0 - safeSqrt(1.0 - c)) * 0.5;
    } else {
        const float c = (viewZenith - zenithHorizon) / max(beta, 1e-6);
        v = safeSqrt(c) * 0.5 + 0.5;
    }
    const float u = azimuth / (2.0 * kPi) + 0.5;
    return unitToSubUv(float2(u, v), size);
}

public void skyViewParams(float2 uv, float r, float ground, float2 size,
                          out float viewZenithCos, out float azimuth, out bool hitsGround) {
    const float2 x = saturate(subUvToUnit(uv, size));
    const float vHorizon = safeSqrt(r * r - ground * ground);
    const float beta = safeAcos(vHorizon / r);
    const float zenithHorizon = kPi - beta;

    float viewZenith;
    if (x.y < 0.5) {
        float c = 1.0 - 2.0 * x.y;
        c = 1.0 - c * c;
        viewZenith = zenithHorizon * c;
        hitsGround = false;
    } else {
        float c = 2.0 * x.y - 1.0;
        c = c * c;
        viewZenith = zenithHorizon + beta * c;
        hitsGround = true;
    }
    viewZenithCos = cos(viewZenith);
    azimuth = (x.x - 0.5) * 2.0 * kPi;
}

// Aerial-perspective atlas, `slices` tiles of `tile` x `tile` texels side by side
// Slice i stores the in-scatter and transmittance from the camera to aerialSliceToDistance(i + 1).
// Distances grow quadratically so the first slices, where the eye notices haze most, are finer.

public float aerialSliceToDistance(float slice, float slices, float maxDistance) {
    const float s = slice / slices;
    return s * s * maxDistance;
}

public float aerialDistanceToSlice(float distance, float slices, float maxDistance) {
    return safeSqrt(distance / maxDistance) * slices;
}

// uv into the atlas for a screen position inside tile `slice`, kept half a texel away from
// the tile edges so bilinear filtering never bleeds into the neighbouring slice.
public float2 aerialAtlasUv(float2 screenUv, int slice, float slices, float tile) {
    const float2 inner = clamp(screenUv, 0.5 / tile, 1.0 - 0.5 / tile);
    return float2((float(slice) + inner.x) / slices, inner.y);
}
