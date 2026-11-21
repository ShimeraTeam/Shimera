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
 * Helpers for shaders that are not the atmosphere itself. Shimera's own materials, and
 * host materials once the host binds the atmosphere's parameters and LUTs.
 *
 * These signatures are the stable public surface of the atmosphere on the GPU side. The
 * LUT layouts behind them may change, the functions here will not.
 */
module atmosphere_apply;

import params;
import lookups;

// Sunlight colour reaching a surface at planet-local position `pos` (km).
public float3 sunTransmittance(AtmosphereParams p, Sampler2D transmittanceLut, float3 pos) {
    const float r = length(pos);
    const float sunCos = dot(pos / r, p.sunDirection.xyz);
    return sampleTransmittance(transmittanceLut, p, r, sunCos);
}

public float3 applyAerialPerspective(AtmosphereParams p, Sampler2D aerialAtlas, float3 color,
                                     float2 screenUv, float distance) {
    const float4 ap = sampleAerial(aerialAtlas, p, screenUv, distance);
    return color * ap.a + ap.rgb;
}
