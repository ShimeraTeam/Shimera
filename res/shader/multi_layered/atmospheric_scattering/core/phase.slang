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
 * Phase functions, how much of the light hitting a particle leaves in a given direction.
 * All of them integrate to 1 over the sphere.
 *
 * mu is the cosine of the angle between the view direction (camera -> sample) and the
 * direction towards the sun. Looking straight at the sun gives mu = 1, and the forward
 * peak of the Mie functions sits there, which is what puts the glow around the sun.
 */
module phase;

import geometry;

public static const int kPhaseNone = 0; // absorbs only, scatters nothing
public static const int kPhaseRayleigh = 1; // molecules
public static const int kPhaseCornetteShanks = 2; // aerosols, the usual Mie approximation
public static const int kPhaseHenyeyGreenstein = 3; // aerosols, cheaper and softer
public static const int kPhaseIsotropic = 4; // debugging and stylised media

public float phaseRayleigh(float mu) {
    return 3.0 / (16.0 * kPi) * (1.0 + mu * mu);
}

// g in (-1, 1), positive scatters forward. (Henyey & Greenstein 1941)
public float phaseHenyeyGreenstein(float mu, float g) {
    const float g2 = g * g;
    const float denom = 1.0 + g2 - 2.0 * g * mu;
    return (1.0 - g2) / (4.0 * kPi * denom * sqrt(denom));
}

// Henyey-Greenstein shaped by the Rayleigh (1 + mu^2) lobe. (Cornette & Shanks 1992)
public float phaseCornetteShanks(float mu, float g) {
    const float g2 = g * g;
    const float denom = 1.0 + g2 - 2.0 * g * mu;
    return 3.0 / (8.0 * kPi) * ((1.0 - g2) * (1.0 + mu * mu)) / ((2.0 + g2) * denom * sqrt(denom));
}

public float phaseIsotropic() {
    return 1.0 / (4.0 * kPi);
}

public float evaluatePhase(int id, float mu, float g) {
    if (id == kPhaseRayleigh)
        return phaseRayleigh(mu);
    if (id == kPhaseCornetteShanks)
        return phaseCornetteShanks(mu, g);
    if (id == kPhaseHenyeyGreenstein)
        return phaseHenyeyGreenstein(mu, g);
    if (id == kPhaseIsotropic)
        return phaseIsotropic();
    return 0.0;
}
