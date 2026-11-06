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
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "ShaderEffect.inl"
#include "profiles/AtmosphereProfile.hpp"
#include "scene/Camera.hpp"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"
#include "uniform/Vec4.inl"

namespace shimera {

/**
 * Physically based sky and atmosphere, after Hillaire 2020, "A Scalable and Production
 * Ready Sky and Atmosphere Rendering Technique".
 *
 * A post-process, it needs the scene colour, the scene depth, the camera, and where the
 * planet is. Four small lookup tables carry the expensive light transport, two depend on
 * the medium only and are rebuilt when the profile changes, two depend on the camera and
 * are rebuilt every frame. The screen pass then mostly reads textures.
 *
 * Shaders live in res/shader/multi_layered/atmospheric_scattering/.
 */
class AtmosphericScattering final : public ShaderEffect<AtmosphericScattering> {
    public:
        /* Physical: the planet profile keep real world behaviour.
         * Normalized: keep sky's look the same no matter the size
         * Sky: no planet in your world, only its sky (see `withSkyMode()`). */
        enum class ScaleMode { Physical, Normalized, Sky };

        // Useful to check different LUTs and see work separated.
        enum class DebugMode {
            Off = 0,
            Reference = 1,
            TransmittanceLut = 2,
            MultiScatterLut = 3,
            SkyViewLut = 4,
            AerialLut = 5,
            Altitude = 6,
            Depth = 7,
        };

        // How the host's depth buffer is encoded. OpenGL defaults.
        struct DepthConvention {
            enum class Range { NegOneToOne, ZeroToOne };
            Range range = Range::NegOneToOne;
            bool reversed = false;
            bool flipY = false;
        };

        struct Quality {
            int transmittanceSteps = 40;
            int multiScatterDirections = 64;
            int multiScatterSteps = 20;
            int skyViewSamples = 30;
            int aerialSamples = 16;
            int spaceSamples = 24; // per pixel, camera outside or geometry out of range
            int referenceSamples = 128;
            bool jitter = true;
        };

        explicit AtmosphericScattering(AtmosphereProfile profile = AtmospherePresets::earth());

        void render(const GLTexture& input) override;
        void render(const GLTexture& input, GLFramebuffer& target) override;
        void updateUniforms() override;
        void resize(int width, int height) override;

        [[nodiscard]] std::string getName() const override { return "AtmosphericScattering"; }
        [[nodiscard]] bool needsDepth() const override { return true; }

        AtmosphericScattering& withCamera(const Camera& camera);
        AtmosphericScattering& withDepthConvention(DepthConvention convention);

        // Normalized mode
        AtmosphericScattering& withPlanet(Vec3<double> center, double radius, double atmosphereHeight);
        // Physical mode
        AtmosphericScattering& withPhysicalPlanet(Vec3<double> center, double unitsPerKilometer);

        // Sky mode
        AtmosphericScattering& withSkyMode(double seaLevel = 0.0, double unitsPerKilometer = 1000.0);
        // The world's up and north, for sky mode and withSunAngles.
        AtmosphericScattering& withWorldAxes(Vec3<float> up, Vec3<float> north);
        /* Haze is physical, so it only shows over kilometres. This multiplies the distance
         * to your geometry as the haze sees it. 20 makes 500 m of your world look like
         * 10 km of air. 1 by default. */
        AtmosphericScattering& withAerialDistanceScale(float scale);
        // Show the invisible planet's lit ground below the horizon, where you draw nothing.
        AtmosphericScattering& withVirtualGround(bool visible);

        // World-space direction towards the sun. Normalised internally.
        AtmosphericScattering& withSunDirection(Vec3<float> toSun);
        // The sun by angles against the world axes (withWorldAxes).
        AtmosphericScattering& withSunAngles(float elevationDegrees, float azimuthDegrees);
        AtmosphericScattering& withSunIlluminance(Vec3<float> illuminance);
        AtmosphericScattering& withSunDisk(float angularRadiusDegrees, float intensity = 1.0f,
                                           float limbDarkening = 0.6f);

        AtmosphericScattering& withProfile(const AtmosphereProfile& profile);
        // To edit profile directly.
        [[nodiscard]] AtmosphereProfile& profile() { return m_profile; }
        AtmosphericScattering& withExposure(float exposure);
        // True when the scene texture holds linear radiance, false for display-encoded 8-bit.
        AtmosphericScattering& withSceneLinear(bool linear);
        // Force HDR or display output, nullopt follows the pipeline (the default).
        AtmosphericScattering& withOutputHdr(std::optional<bool> hdr);
        AtmosphericScattering& withQuality(Quality quality);
        AtmosphericScattering& withDebugMode(DebugMode mode);
        AtmosphericScattering& withDebugExposure(float exposure);

        // To get the profile (different than `profile()`).
        [[nodiscard]] const AtmosphereProfile& getProfile() const { return m_profile; }
        [[nodiscard]] ScaleMode getScaleMode() const { return m_scaleMode; }
        [[nodiscard]] Vec3<float> getSunDirection() const { return m_sunDirection; }
        // The current sun as angles against the world axes, whichever way it was set.
        [[nodiscard]] float getSunElevation() const;
        [[nodiscard]] float getSunAzimuth() const;
        [[nodiscard]] float getExposure() const { return m_exposure; }
        [[nodiscard]] Quality getQuality() const { return m_quality; }
        [[nodiscard]] DebugMode getDebugMode() const { return m_debugMode; }
        // Kilometres per world unit after scaling, so what the shaders see.
        [[nodiscard]] float getKmPerUnit() const { return m_frame.kmPerUnit; }
        [[nodiscard]] bool isCameraInsideAtmosphere() const { return m_frame.inside; }

    private:
        struct LutPass {
            std::unique_ptr<GLFramebuffer> target;
            std::unique_ptr<GLPostProcessor> pass;
            int width = 0;
            int height = 0;
        };

        // Everything derived from the public parameters, recomputed once per frame.
        struct Frame {
            float groundRadius = 6360.0f; // km
            float topRadius = 6460.0f; // km
            float kmPerUnit = 1.0f;
            float mediumScale = 1.0f; // Normalized mode, coefficient multiplier
            Vec3<float> cameraPosKm{0.0f};
            Vec3<float> sunDirection{0.0f, 1.0f, 0.0f};
            Mat4 invProjection = Mat4::identity();
            Mat4 viewToPlanet = Mat4::identity();
            bool inside = false;
        };

        void renderImpl(const GLTexture& input, uint32_t target);
        void computeFrame();
        void uploadAtmosphere(GLPostProcessor& pass) const;
        void uploadView(GLPostProcessor& pass) const;
        void runLutPass(LutPass& lut, const GLTexture& dummyInput);
        [[nodiscard]] std::vector<float> mediumSnapshot() const;
        [[nodiscard]] MediumComponent scaledComponent(std::size_t index) const;

        static LutPass makeLut(const char* shader, int width, int height);

        // Unit up, north (made perpendicular to up) and east of the world axes.
        void worldBasis(Vec3<float>& up, Vec3<float>& north, Vec3<float>& east) const;
        [[nodiscard]] Vec3<float> directionFromAngles(float elevationDegrees, float azimuthDegrees) const;

        AtmosphereProfile m_profile;
        ScaleMode m_scaleMode = ScaleMode::Normalized;
        Vec3<double> m_planetCenter{0.0};
        double m_planetRadiusUnits = 100.0;
        double m_atmosphereHeightUnits = 3.0;
        double m_unitsPerKilometer = 1000.0;
        double m_seaLevel = 0.0;
        Vec3<float> m_worldUp{0.0f, 1.0f, 0.0f};
        Vec3<float> m_worldNorth{0.0f, 0.0f, -1.0f};
        float m_aerialDistanceScale = 1.0f;
        std::optional<bool> m_virtualGround; // nullopt, follows the scale mode

        Camera m_camera = Camera::fromMatrices(Mat4::identity(), Mat4::identity(), Vec3(0.0f));
        DepthConvention m_depthConvention{};
        Vec3<float> m_sunDirection{0.3f, 0.5f, 0.8f};
        // Set by withSunAngles, so the sun follows the axes if they change afterwards.
        std::optional<std::pair<float, float>> m_sunAngles;
        Vec3<float> m_sunIlluminance{1.0f};
        float m_sunAngularRadiusDeg = 0.545f;
        float m_sunDiskIntensity = 1.0f;
        float m_sunLimbDarkening = 0.6f;
        float m_exposure = 1.0f;
        float m_debugExposure = 1.0f;
        bool m_sceneLinear = false;
        std::optional<bool> m_outputHdr;
        Quality m_quality{};
        DebugMode m_debugMode = DebugMode::Off;

        LutPass m_transmittance;
        LutPass m_multiScatter;
        LutPass m_skyView;
        LutPass m_aerial;
        static constexpr int kAerialSlices = 32;
        static constexpr int kAerialTile = 32;

        Frame m_frame;
        std::vector<float> m_uploadedMedium; // what the medium LUTs were last built from
};

}
