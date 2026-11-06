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
 * Composite pass, puts the atmosphere over the scene, once per pixel.
 *
 * Output is HDR radiance when the pipeline is HDR, otherwise exposure + tone curve +
 * display encode happen here, with the scene decoded first so sky and scene mix linearly.
 *
 * Debug modes (u_debugMode):
 *   1 brute-force reference: ray march every pixel with many samples, no sky-view / aerial
 *   2..5 show the transmittance, multi-scattering, sky-view and aerial LUTs
 *   6 input sanity: altitude of each pixel's surface, sky in blue, below ground in red
 *   7 raw depth: red = the stored value, green = 1000 x its distance from the far plane
 */
#include "postprocess.slang"

import params;
import platform;
import geometry;
import lut_mapping;
import lookups;
import raymarch;
import atmosphere_apply;

uniform Sampler2D u_screenTexture;
uniform Sampler2D u_depthTexture;
uniform Sampler2D u_transmittanceLut;
uniform Sampler2D u_multiScatterLut;
uniform Sampler2D u_skyViewLut;
uniform Sampler2D u_aerialLut;

uniform AtmosphereParams u_atm;
uniform ViewParams u_view;
uniform float2 u_resolution;
uniform int u_spaceSamples;
uniform int u_referenceSamples;
uniform int u_debugMode;
uniform int u_outputHdr;
uniform int u_sceneIsLinear;
uniform int u_jitter;
uniform float u_exposure;
uniform float u_debugExposure;
uniform int u_virtualGround; // 0 = below the horizon, show the horizon's colour instead of the ground

// Interleaved gradient noise, a cheap screen-space pattern for sample jitter. (Jimenez 2014)
float interleavedGradientNoise(float2 pixel) {
    const float f = 0.06711056 * pixel.x + 0.00583715 * pixel.y;
    return frac(52.9829189 * frac(f));
}

// ACES filmic fit. Same curve as the Tonemap effect, for the LDR path. (Narkowicz 2016)
float3 acesFilmic(float3 x) {
    return saturate((x * (2.51 * x + 0.03)) / (x * (2.43 * x + 0.59) + 0.14));
}

float3 decodeScene(float3 c) {
    return u_sceneIsLinear != 0 ? c : pow(max(c, 0.0), 2.2);
}

// LDR output is display-ready, tone curve then the display encode, whatever the input was.
float4 finish(float3 linearColor, float alpha) {
    if (u_outputHdr != 0)
        return float4(linearColor, alpha);
    const float3 c = acesFilmic(linearColor * u_exposure);
    return float4(pow(c, 1.0 / 2.2), alpha);
}

/* The sun as a disk of angular radius sunAngularRadius with a soft edge and simple limb
 * darkening. */
float3 sunDisk(float3 dir) {
    const float cosRadius = cos(u_atm.sunAngularRadius);
    const float cosAngle = dot(dir, u_atm.sunDirection.xyz);
    const float softness = (1.0 - cosRadius) * 0.125;
    const float edge = smoothstep(cosRadius - softness, cosRadius + softness, cosAngle);
    if (edge <= 0.0)
        return float3(0.0);
    const float sinRadius = safeSqrt(1.0 - cosRadius * cosRadius);
    const float radiance = 1.0 / (kPi * max(sinRadius * sinRadius, 1e-8));
    // Fraction of the radius from the centre, 0 at the centre, 1 at the rim.
    const float t = saturate((1.0 - cosAngle) / max(1.0 - cosRadius, 1e-8));
    const float limb = 1.0 - u_atm.sunDisk.y * (1.0 - safeSqrt(1.0 - t * t));
    return u_atm.sunIlluminance.rgb * (radiance * u_atm.sunDisk.x * limb * edge);
}

/* `dir` raised to just above the horizon, keeping its compass direction. Used when the
 * virtual planet's ground should not show. */
float3 liftAboveHorizon(float3 dir, float r) {
    const float3 up = cameraUp(u_atm);
    float3 flat = dir - up * dot(dir, up);
    if (dot(flat, flat) < 1e-10)
        flat = abs(up.x) < 0.9 ? cross(up, float3(1.0, 0.0, 0.0)) : cross(up, float3(0.0, 0.0, 1.0));
    const float c = horizonCos(r, u_atm.groundRadius) + 1e-3;
    return up * c + normalize(flat) * safeSqrt(1.0 - c * c);
}

float4 debugView(float2 uv, bool isSky, float3 origin, float3 dir, float geomDistance) {
    if (u_debugMode == 2)
        return float4(u_transmittanceLut.Sample(uv).rgb * u_debugExposure, 1.0);
    if (u_debugMode == 3)
        return float4(u_multiScatterLut.Sample(uv).rgb * u_debugExposure, 1.0);
    if (u_debugMode == 4)
        return float4(u_skyViewLut.Sample(uv).rgb * u_debugExposure, 1.0);
    if (u_debugMode == 5)
        return float4(u_aerialLut.Sample(uv).rgb * u_debugExposure, u_aerialLut.Sample(uv).a);
    if (u_debugMode == 7) {
        const float raw = u_depthTexture.Sample(uv).r;
        return float4(raw, saturate((1.0 - raw) * 1000.0), isSky ? 1.0 : 0.0, 1.0);
    }

    if (isSky)
        return float4(0.1, 0.2, 0.8, 1.0);
    const float3 pos = origin + dir * geomDistance;
    const float h = length(pos) - u_atm.groundRadius;
    if (h < -(u_atm.topRadius - u_atm.groundRadius) * 0.05)
        return float4(0.8, 0.1, 0.1, 1.0);
    const float band = frac(h / ((u_atm.topRadius - u_atm.groundRadius) * 0.1));
    return float4(band, band, band, 1.0);
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float2 uv = input.texCoords;
    const float4 scene = u_screenTexture.Sample(uv);
    const float raw = u_depthTexture.Sample(uv).r;

    const DepthConvention conv = depthConvention(u_view);
    const bool isSky = isFarPlane(raw, conv);

    const float3 viewDir = viewRayDirection(uv, u_view.invProjection, conv);
    const float3 dir = normalize(mul(u_view.viewToPlanet, float4(viewDir, 0.0)).xyz);
    const float3 origin = u_atm.cameraPos.xyz;

    float geomDistance = 1e30;
    if (!isSky) {
        const float3 viewPos = viewPositionFromDepth(uv, raw, u_view.invProjection, conv);
        geomDistance = length(viewPos) * u_view.scale.x * u_view.scale.y;
    }

    if (u_debugMode >= 2)
        return debugView(uv, isSky, origin, dir, geomDistance);

    const float r = length(origin);
    const bool inside = r < u_atm.topRadius;

    // Where the ray crosses the atmosphere, clipped to the scene's own geometry.
    float tTop0, tTop1;
    const bool hitsAtmosphere = raySphere(origin, dir, u_atm.topRadius, tTop0, tTop1) && tTop1 > 0.0;

    float3 sceneLinear = decodeScene(scene.rgb);

    if (!hitsAtmosphere) {
        float3 color = sceneLinear;
        if (isSky)
            color += sunDisk(dir);
        return finish(color, scene.a);
    }

    const float tStart = max(tTop0, 0.0);
    float tEnd = min(tTop1, geomDistance);

    /* The planet itself, as a sphere, in case the host draws nothing there or draws it
     * slightly smaller. */
    float tGround0, tGround1;
    const bool hitsGround = raySphere(origin, dir, u_atm.groundRadius, tGround0, tGround1)
                            && tGround0 > 0.0 && tGround0 < tEnd
                            && (isSky || !isFlatWorld(u_view));
    const bool virtualGround = hitsGround && isSky;
    if (hitsGround)
        tEnd = tGround0;

    float3 L = float3(0.0);
    float3 T = float3(1.0);

    if (tEnd > tStart) {
        const float jitter = u_jitter != 0 ? interleavedGradientNoise(uv * u_resolution) : 0.5;

        if (u_debugMode == 1) {
            const ScatteringResult ref = integrateScattering(u_atm, u_transmittanceLut, u_multiScatterLut,
                                                             origin + dir * tStart, dir, tEnd - tStart,
                                                             u_referenceSamples, jitter, virtualGround);
            L = ref.luminance;
            T = ref.transmittance;
        } else if (inside && isSky) {
            const bool hideGround = hitsGround && u_virtualGround == 0;
            const float3 skyDir = hideGround ? liftAboveHorizon(dir, clampedRadius(u_atm, r)) : dir;
            L = sampleSkyView(u_skyViewLut, u_atm, skyDir);
            T = float3(0.0);
            if (!hitsGround) {
                // Starfields and such behind the sky still show through the air.
                const float rc = clampedRadius(u_atm, r);
                T = sampleTransmittance(u_transmittanceLut, u_atm, rc, dot(cameraUp(u_atm), dir));
            }
        } else if (inside && !isSky && geomDistance <= u_atm.aerial.z) {
            const float4 ap = sampleAerial(u_aerialLut, u_atm, uv, geomDistance);
            L = ap.rgb;
            T = float3(ap.a);
        } else {
            const ScatteringResult space = integrateScattering(u_atm, u_transmittanceLut, u_multiScatterLut,
                                                               origin + dir * tStart, dir, tEnd - tStart,
                                                               u_spaceSamples, jitter, virtualGround);
            L = space.luminance;
            T = space.transmittance;
        }
    }

    float3 color = sceneLinear * T + L;

    // The sun disk, seen through whatever air lies in front of it. Hidden by the planet.
    if (isSky && !hitsGround) {
        float3 sunT = T;
        if (inside) {
            const float rc = clampedRadius(u_atm, r);
            sunT = sampleTransmittance(u_transmittanceLut, u_atm, rc, dot(cameraUp(u_atm), dir));
        }
        color += sunDisk(dir) * sunT;
    }

    return finish(color, scene.a);
}
