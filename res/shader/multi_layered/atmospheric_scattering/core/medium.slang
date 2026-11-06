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
 * The participating medium as data, not code.
 *
 * An atmosphere is up to four components (molecules, aerosols, ozone, one custom), each
 * with scattering and absorption coefficients at sea level (per km), a density profile
 * giving how those coefficients fall off with altitude, and a phase function.
 *
 * The density profile follows the generic two-layer form from Bruneton's 2017
 * implementation of "Precomputed Atmospheric Scattering"
 *
 * Only float4 vectors appear in the structs, std140 and the Metal/Vulkan layouts agree on
 * them, which is not true of float3.
 */
module medium;

import phase;

public struct DensityLayer {
    public float width; // km
    public float expTerm;
    public float expScale; // 1/km
    public float linearTerm; // 1/km
    public float constantTerm;
};

public struct MediumComponent {
    public float4 scattering; // rgb
    public float4 absorption; // rgb
    public DensityLayer layers[2];
    public int phaseId; // kPhase* from phase.slang
    public float phaseParam; // anisotropy g for the Mie functions
};

public float evaluateDensity(DensityLayer l, float h) {
    return saturate(l.expTerm * exp(l.expScale * h) + l.linearTerm * h + l.constantTerm);
}

public float componentDensity(MediumComponent c, float h) {
    return h < c.layers[0].width ? evaluateDensity(c.layers[0], h) : evaluateDensity(c.layers[1], h);
}

// Total extinction (1/km) at altitude h. What the transmittance LUT integrates.
public float3 extinctionAt(MediumComponent components[4], float h) {
    float3 sigmaT = float3(0.0);
    for (int i = 0; i < 4; ++i) {
        const float d = componentDensity(components[i], h);
        const float3 scattering = components[i].phaseId == kPhaseNone ? float3(0.0) : components[i].scattering.rgb;
        sigmaT += (scattering + components[i].absorption.rgb) * d;
    }
    return sigmaT;
}

/* Everything a ray-march sample needs at altitude h, seen under view/sun cosine mu.
 * sigmaS is total scattering, used with the (isotropic) multiple-scattering term
 * sigmaSPhased is the sum of scattering * phase(mu) per component, used with direct sunlight
 * sigmaT is the extinction */
public void sampleMedium(MediumComponent components[4], float h, float mu,
                         out float3 sigmaS, out float3 sigmaSPhased, out float3 sigmaT) {
    sigmaS = float3(0.0);
    sigmaSPhased = float3(0.0);
    sigmaT = float3(0.0);
    for (int i = 0; i < 4; ++i) {
        const float d = componentDensity(components[i], h);
        const bool scatters = components[i].phaseId != kPhaseNone;
        const float3 scattering = scatters ? components[i].scattering.rgb * d : float3(0.0);
        sigmaS += scattering;
        sigmaSPhased += scattering * evaluatePhase(components[i].phaseId, mu, components[i].phaseParam);
        sigmaT += scattering + components[i].absorption.rgb * d;
    }
}
