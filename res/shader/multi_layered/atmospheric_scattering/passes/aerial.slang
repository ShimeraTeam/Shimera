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
 * Aerial-perspective pass, the haze between the camera and geometry, as a coarse grid of
 * screen position x depth (Hillaire 2020, section 5.5). Due to GL 3.3's lack of 3D targets,
 * aerial perspective uses a screen-depth atlas. Each tile stores RGB in-scatter and mean
 * transmittance up to distance i+1. Fragments independently ray-march per texel without slice accumulation.
 */
#include "postprocess.slang"

import params;
import platform;
import geometry;
import lut_mapping;
import raymarch;

uniform Sampler2D u_transmittanceLut;
uniform Sampler2D u_multiScatterLut;
uniform AtmosphereParams u_atm;
uniform ViewParams u_view;
uniform int u_samples;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float slices = u_atm.aerial.x;
    const float maxDistance = u_atm.aerial.z;

    // Which tile, and where inside it.
    const float x = input.texCoords.x * slices;
    const float slice = floor(x);
    const float2 screenUv = float2(x - slice, input.texCoords.y);

    const DepthConvention conv = depthConvention(u_view);
    const float3 viewDir = viewRayDirection(screenUv, u_view.invProjection, conv);
    const float3 dir = normalize(mul(u_view.viewToPlanet, float4(viewDir, 0.0)).xyz);
    const float3 origin = u_atm.cameraPos.xyz;

    const float4 nothing = float4(0.0, 0.0, 0.0, 1.0);

    float tTop0, tTop1;
    if (!raySphere(origin, dir, u_atm.topRadius, tTop0, tTop1) || tTop1 <= 0.0)
        return nothing;

    float tStart = max(tTop0, 0.0);
    float tEnd = min(tTop1, aerialSliceToDistance(slice + 1.0, slices, maxDistance));

    // The froxels only ever serve geometry, which in sky mode lies on the host's flat ground.
    float tGround0, tGround1;
    if (!isFlatWorld(u_view) && raySphere(origin, dir, u_atm.groundRadius, tGround0, tGround1) && tGround0 > 0.0)
        tEnd = min(tEnd, tGround0);

    if (tEnd <= tStart)
        return nothing;

    const ScatteringResult result = integrateScattering(u_atm, u_transmittanceLut, u_multiScatterLut,
                                                        origin + dir * tStart, dir, tEnd - tStart,
                                                        u_samples, 0.5, false);
    const float meanT = dot(result.transmittance, float3(1.0 / 3.0));
    return float4(result.luminance, meanT);
}
