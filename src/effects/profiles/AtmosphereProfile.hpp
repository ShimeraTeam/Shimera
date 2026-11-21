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

#pragma once

#include <array>

#include "uniform/Vec3.inl"

namespace shimera {

/**
 * What an atmosphere is made of, as plain data. Units are kilometres and 1/km, for an
 * atmosphere of the size given by groundRadiusKm and atmosphereHeightKm. The effect
 * rescales everything when the planet it is applied to has another size (see
 * AtmosphericScattering::withPlanet), so a profile authored for Earth keeps its look on a
 * two-kilometre toy planet.
 *
 * Mirrors the structs in res/shader/multi_layered/atmospheric_scattering/core/medium.slang.
 */

struct DensityLayer {
    float width = 0.0f; // km, see DensityProfile
    float expTerm = 0.0f;
    float expScale = 0.0f; // 1/km
    float linearTerm = 0.0f; // 1/km
    float constantTerm = 0.0f;

    static DensityLayer exponential(float scaleHeightKm) {
        return {.width = 0.0f, .expTerm = 1.0f, .expScale = -1.0f / scaleHeightKm};
    }
    static DensityLayer constant(float value = 1.0f) {
        return {.width = 0.0f, .constantTerm = value};
    }
    static DensityLayer ramp(float h0Km, float h1Km) {
        const float slope = 1.0f / (h1Km - h0Km);
        return {.width = 0.0f, .linearTerm = slope, .constantTerm = -h0Km * slope};
    }
};

/* layers[0] applies below layers[0].width km, layers[1] above. A single-layer
 * profile sets width to 0 and puts everything in layers[1]. */
using DensityProfile = std::array<DensityLayer, 2>;

enum class PhaseFunction : int {
    None = 0, // absorbs only (ozone)
    Rayleigh = 1, // molecules
    CornetteShanks = 2, // aerosols, the usual Mie approximation
    HenyeyGreenstein = 3, // aerosols, softer and cheaper
    Isotropic = 4, // stylised media
};

struct MediumComponent {
    Vec3<float> scattering{0.0f};
    Vec3<float> absorption{0.0f};
    DensityProfile density{};
    PhaseFunction phase = PhaseFunction::None;
    float anisotropy = 0.0f; // g of the Mie phase functions, (-1, 1)
};

struct AtmosphereProfile {
    static constexpr std::size_t kRayleigh = 0;
    static constexpr std::size_t kMie = 1;
    static constexpr std::size_t kOzone = 2;
    static constexpr std::size_t kCustom = 3;

    float groundRadiusKm = 6360.0f;
    float atmosphereHeightKm = 100.0f;
    Vec3<float> groundAlbedo{0.3f};
    std::array<MediumComponent, 4> components{};
    float multiScatteringFactor = 1.0f; // 1 = physical, 0 = single scattering only

    // Artistic overrides, applied on top of the physical values at upload time.
    Vec3<float> rayleighTint{1.0f};
    float hazeAmount = 1.0f;
    float ozoneAmount = 1.0f;
    float densityScale = 1.0f;
    // Stretches the air vertically without changing how much of it there is.
    float heightScale = 1.0f;

    float aerialMaxDistanceKm = 128.0f;
};

namespace AtmospherePresets {

// Hillaire 2020 / Bruneton 2017 Earth. Rayleigh, Mie with absorption, ozone tent at 25 km.
inline AtmosphereProfile earth() {
    AtmosphereProfile p;

    MediumComponent& rayleigh = p.components[AtmosphereProfile::kRayleigh];
    rayleigh.scattering = {5.802e-3f, 13.558e-3f, 33.1e-3f};
    rayleigh.density[1] = DensityLayer::exponential(8.0f);
    rayleigh.phase = PhaseFunction::Rayleigh;

    MediumComponent& mie = p.components[AtmosphereProfile::kMie];
    mie.scattering = Vec3(3.996e-3f);
    mie.absorption = Vec3(4.40e-3f);
    mie.density[1] = DensityLayer::exponential(1.2f);
    mie.phase = PhaseFunction::CornetteShanks;
    mie.anisotropy = 0.8f;

    MediumComponent& ozone = p.components[AtmosphereProfile::kOzone];
    ozone.absorption = {0.650e-3f, 1.881e-3f, 0.085e-3f};
    ozone.density[0] = DensityLayer::ramp(10.0f, 25.0f); // rising half of the tent
    ozone.density[0].width = 25.0f;
    ozone.density[1] = DensityLayer::ramp(40.0f, 25.0f); // falling half
    ozone.phase = PhaseFunction::None;

    return p;
}

inline AtmosphereProfile earthHazy() {
    AtmosphereProfile p = earth();
    p.hazeAmount = 3.0f;
    return p;
}

// A thin, dusty, CO2 atmosphere. Little molecular scattering, a lot of reddish dust.
inline AtmosphereProfile mars() {
    AtmosphereProfile p;
    p.groundRadiusKm = 3390.0f;
    p.atmosphereHeightKm = 80.0f;
    p.groundAlbedo = {0.25f, 0.15f, 0.10f};

    MediumComponent& rayleigh = p.components[AtmosphereProfile::kRayleigh];
    rayleigh.scattering = {0.6e-3f, 1.3e-3f, 3.1e-3f};
    rayleigh.density[1] = DensityLayer::exponential(11.0f);
    rayleigh.phase = PhaseFunction::Rayleigh;

    MediumComponent& dust = p.components[AtmosphereProfile::kMie];
    dust.scattering = {6.5e-3f, 4.5e-3f, 3.0e-3f};
    dust.absorption = {0.6e-3f, 1.8e-3f, 3.5e-3f};
    dust.density[1] = DensityLayer::exponential(10.0f);
    dust.phase = PhaseFunction::CornetteShanks;
    dust.anisotropy = 0.7f;

    return p;
}

}

}
