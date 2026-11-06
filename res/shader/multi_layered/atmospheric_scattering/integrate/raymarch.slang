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
 * The one ray march every pass shares.
 */
module raymarch;

import params;
import geometry;
import phase;
import medium;
import lookups;

public struct ScatteringResult {
    public float3 luminance;
    public float3 transmittance;
};

float3 groundBounce(AtmosphereParams p, Sampler2D transmittanceLut, float3 pos, float3 illuminance) {
    const float r = length(pos);
    const float3 up = pos / r;
    const float sunCos = dot(up, p.sunDirection.xyz);
    const float3 sunT = sampleTransmittance(transmittanceLut, p, r, sunCos);
    return p.groundAlbedo.rgb / kPi * sunT * saturate(sunCos) * illuminance;
}

/* Integrates from `origin` along `dir` (both planet-local km) over [0, maxDistance]. The
 * caller clamps maxDistance to the atmosphere, the ground and the depth buffer. */
public ScatteringResult integrateScattering(AtmosphereParams p, Sampler2D transmittanceLut,
                                            Sampler2D multiScatterLut, float3 origin, float3 dir,
                                            float maxDistance, int sampleCount, float jitter,
                                            bool addGroundBounce) {
    float3 L = float3(0.0);
    float3 T = float3(1.0);

    const float3 sun = p.sunDirection.xyz;
    const float mu = dot(dir, sun);
    const float dt = maxDistance / float(sampleCount);

    for (int i = 0; i < sampleCount; ++i) {
        const float t = (float(i) + jitter) * dt;
        const float3 pos = origin + dir * t;
        const float r = max(length(pos), p.groundRadius + 1e-4);
        const float h = r - p.groundRadius;
        const float3 up = normalize(pos);
        const float sunCos = dot(up, sun);

        float3 sigmaS, sigmaSPhased, sigmaT;
        sampleMedium(p.components, h, mu, sigmaS, sigmaSPhased, sigmaT);

        // The planet's own shadow, no direct sun below the horizon of this sample.
        const float shadow = intersectsGround(r, sunCos, p.groundRadius) ? 0.0 : 1.0;
        const float3 sunT = sampleTransmittance(transmittanceLut, p, r, sunCos);
        const float3 psi = sampleMultiScatter(multiScatterLut, p, r, sunCos) * p.multiScatteringFactor;

        const float3 S = p.sunIlluminance.rgb * (sunT * shadow * sigmaSPhased + psi * sigmaS);
        const float3 stepT = exp(-sigmaT * dt);
        const float3 Sint = (S - S * stepT) / max(sigmaT, float3(1e-7));

        L += T * Sint;
        T *= stepT;
    }

    if (addGroundBounce)
        L += T * groundBounce(p, transmittanceLut, origin + dir * maxDistance, p.sunIlluminance.rgb);

    ScatteringResult result;
    result.luminance = L;
    result.transmittance = T;
    return result;
}

/* The integrand of the multiple-scattering LUT (Hillaire 2020, section 5.3), along one
 * direction from a point, for a sun of unit illuminance and an isotropic phase function */
public void integrateSecondOrder(AtmosphereParams p, Sampler2D transmittanceLut, float3 origin,
                                 float3 dir, float maxDistance, int sampleCount, bool hitsGround,
                                 out float3 secondOrder, out float3 transfer) {
    float3 L = float3(0.0);
    float3 F = float3(0.0);
    float3 T = float3(1.0);

    const float3 sun = p.sunDirection.xyz;
    const float dt = maxDistance / float(sampleCount);
    const float isotropic = phaseIsotropic();

    for (int i = 0; i < sampleCount; ++i) {
        const float t = (float(i) + 0.5) * dt;
        const float3 pos = origin + dir * t;
        const float r = length(pos);
        const float h = r - p.groundRadius;
        const float3 up = pos / r;
        const float sunCos = dot(up, sun);

        float3 sigmaS, sigmaSPhased, sigmaT;
        sampleMedium(p.components, h, 1.0, sigmaS, sigmaSPhased, sigmaT);

        const float shadow = intersectsGround(r, sunCos, p.groundRadius) ? 0.0 : 1.0;
        const float3 sunT = sampleTransmittance(transmittanceLut, p, r, sunCos);

        const float3 S = sunT * shadow * sigmaS * isotropic;
        const float3 stepT = exp(-sigmaT * dt);
        const float3 safeT = max(sigmaT, float3(1e-7));

        L += T * (S - S * stepT) / safeT;
        F += T * (sigmaS - sigmaS * stepT) / safeT;
        T *= stepT;
    }

    if (hitsGround)
        L += T * groundBounce(p, transmittanceLut, origin + dir * maxDistance, float3(1.0));

    secondOrder = L;
    transfer = F;
}
