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
 * Multiple-scattering LUT pass (Hillaire 2020, section 5.3).
 *
 * Higher-order light transport sums to L2 / (1 - f_ms) assuming isotropic scattering and
 * uniform neighbor lighting. GL 3.3 lacks compute shaders, so fragments loop over 64 directions
 * instead of threads. The atlas uses only 1024 texels, rebuilt rarely.
 */
#include "postprocess.slang"

import params;
import geometry;
import lut_mapping;
import raymarch;

uniform Sampler2D u_transmittanceLut;
uniform AtmosphereParams u_atm;
uniform int u_directions;
uniform int u_steps;

// Point i of a Fibonacci sphere, evenly spread directions with no clustering at the poles.
float3 fibonacciDirection(int i, int count) {
    const float goldenAngle = 2.39996322972865; // pi * (3 - sqrt(5))
    const float z = 1.0 - 2.0 * (float(i) + 0.5) / float(count);
    const float radius = safeSqrt(1.0 - z * z);
    const float phi = float(i) * goldenAngle;
    return float3(radius * cos(phi), radius * sin(phi), z);
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float r, sunCos;
    multiScatterParams(input.texCoords, u_atm.groundRadius, u_atm.topRadius,
                       u_atm.multiScatterLutSize.xy, r, sunCos);

    /* The LUT is indexed by [altitude, sun angle] only, so pick any point with that altitude
     * and a sun direction with that angle, up is +y here. The integrator reads the sun from
     * the params, so a local copy of them carries this texel's sun. */
    AtmosphereParams local = u_atm;
    local.sunDirection = float4(safeSqrt(1.0 - sunCos * sunCos), sunCos, 0.0, 0.0);
    local.cameraPos = float4(0.0, r, 0.0, 0.0);
    const float3 origin = local.cameraPos.xyz;

    float3 secondOrder = float3(0.0);
    float3 transfer = float3(0.0);

    for (int i = 0; i < u_directions; ++i) {
        const float3 dir = fibonacciDirection(i, u_directions);
        const float mu = dir.y; // cos to up at the origin
        const bool hitsGround = intersectsGround(r, mu, local.groundRadius);
        const float maxDistance = hitsGround ? distanceToGround(r, mu, local.groundRadius)
                                             : distanceToTop(r, mu, local.topRadius);

        float3 L, F;
        integrateSecondOrder(local, u_transmittanceLut, origin, dir, maxDistance, u_steps, hitsGround, L, F);
        secondOrder += L;
        transfer += F;
    }

    // Uniform directions, the average over the sphere.
    secondOrder /= float(u_directions);
    transfer /= float(u_directions);

    // Geometric series of all orders >= 2.
    const float3 psi = secondOrder / max(1.0 - min(transfer, 0.99), 1e-4);
    return float4(psi, 1.0);
}
