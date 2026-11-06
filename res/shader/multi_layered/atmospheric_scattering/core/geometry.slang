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
 * Spherical-shell geometry. Everything is planet-local, the planet centre is the origin,
 * distances are kilometres, and a point is described by its radius r and the cosine mu of
 * the angle between the local up and a direction.
 */
module geometry;

public static const float kPi = 3.14159265358979;

public float safeSqrt(float x) {
    return sqrt(max(x, 0.0));
}

public float safeAcos(float x) {
    return acos(clamp(x, -1.0, 1.0));
}

/* Ray against a sphere centred at the origin. t0 <= t1 are the two crossing distances along
 * the ray (either may be negative, meaning behind the origin). False means no crossing. */
public bool raySphere(float3 origin, float3 dir, float radius, out float t0, out float t1) {
    const float b = dot(origin, dir);
    const float c = dot(origin, origin) - radius * radius;
    const float disc = b * b - c;
    if (disc < 0.0) {
        t0 = -1.0;
        t1 = -1.0;
        return false;
    }
    const float s = sqrt(disc);
    t0 = -b - s;
    t1 = -b + s;
    return true;
}

// Distance from (r, mu) to the top of the atmosphere, for r <= top.
public float distanceToTop(float r, float mu, float top) {
    const float disc = r * r * (mu * mu - 1.0) + top * top;
    return max(-r * mu + safeSqrt(disc), 0.0);
}

// Distance from (r, mu) to the ground, only meaningful when intersectsGround() is true.
public float distanceToGround(float r, float mu, float ground) {
    const float disc = r * r * (mu * mu - 1.0) + ground * ground;
    return max(-r * mu - safeSqrt(disc), 0.0);
}

// Does the ray from (r, mu) hit the planet? Only downward-pointing rays can.
public bool intersectsGround(float r, float mu, float ground) {
    return mu < 0.0 && r * r * (mu * mu - 1.0) + ground * ground >= 0.0;
}

// Cosine of the angle between up and the horizon seen from radius r.
public float horizonCos(float r, float ground) {
    return -safeSqrt(1.0 - (ground * ground) / (r * r));
}
