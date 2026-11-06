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
 * Transmittance LUT pass.
 */
#include "postprocess.slang"

import params;
import geometry;
import medium;
import lut_mapping;

uniform AtmosphereParams u_atm;
uniform int u_steps;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float r, mu;
    transmittanceParams(input.texCoords, u_atm.groundRadius, u_atm.topRadius,
                        u_atm.transmittanceLutSize.xy, r, mu);

    const float rayLength = distanceToTop(r, mu, u_atm.topRadius);
    const float dt = rayLength / float(u_steps);

    float3 opticalDepth = float3(0.0);
    for (int i = 0; i < u_steps; ++i) {
        const float t = (float(i) + 0.5) * dt;
        // Radius along the ray from the law of cosines, no 3D positions needed.
        const float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
        opticalDepth += extinctionAt(u_atm.components, ri - u_atm.groundRadius) * dt;
    }

    return float4(exp(-opticalDepth), 1.0);
}
