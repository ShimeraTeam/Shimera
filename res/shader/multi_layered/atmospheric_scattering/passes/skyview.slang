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
 * Sky-view LUT pass, the sky as seen from the camera, in every direction, in a small
 * latitude/longitude map around the local vertical (Hillaire 2020, section 5.4). The sky is
 * low frequency, so ~20k texels ray-marched here replace millions of pixels doing it.
 *
 * Only valid while the camera is inside the atmosphere, the composite ray-marches from space.
 * The sun disk is not in here, the composite adds it at full resolution.
 */
#include "postprocess.slang"

import params;
import geometry;
import lut_mapping;
import lookups;
import raymarch;

uniform Sampler2D u_transmittanceLut;
uniform Sampler2D u_multiScatterLut;
uniform AtmosphereParams u_atm;
uniform int u_samples;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float r = clampedRadius(u_atm, length(u_atm.cameraPos.xyz));

    float viewZenithCos, azimuth;
    bool hitsGround;
    skyViewParams(input.texCoords, r, u_atm.groundRadius, u_atm.skyViewLutSize.xy,
                  viewZenithCos, azimuth, hitsGround);

    float3 up, forward, right;
    skyViewFrame(u_atm, up, forward, right);
    const float3 dir = skyViewDirection(viewZenithCos, azimuth, up, forward, right);
    const float3 origin = up * r;

    const float maxDistance = hitsGround ? distanceToGround(r, viewZenithCos, u_atm.groundRadius)
                                         : distanceToTop(r, viewZenithCos, u_atm.topRadius);

    const ScatteringResult sky = integrateScattering(u_atm, u_transmittanceLut, u_multiScatterLut,
                                                     origin, dir, maxDistance, u_samples, 0.5, hitsGround);
    return float4(sky.luminance, 1.0);
}
