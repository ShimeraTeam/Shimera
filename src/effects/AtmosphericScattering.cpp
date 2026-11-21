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

#include "AtmosphericScattering.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string>

#include "Resources.inl"

using shimera::AtmosphericScattering;
using shimera::MediumComponent;
using shimera::GLTexture;
using shimera::Vec3;
using shimera::Vec4;

namespace {

constexpr int kTransmittanceWidth = 256;
constexpr int kTransmittanceHeight = 64;
constexpr int kMultiScatterSize = 32;
constexpr int kSkyViewWidth = 192;
constexpr int kSkyViewHeight = 108;

// Texture units of the composite and LUT passes. Unit 0 is the scene, by GLPostProcessor.
constexpr int kUnitDepth = 1;
constexpr int kUnitTransmittance = 2;
constexpr int kUnitMultiScatter = 3;
constexpr int kUnitSkyView = 4;
constexpr int kUnitAerial = 5;

Vec4<float> xyz0(const Vec3<float>& v) {
    return {v.x, v.y, v.z, 0.0f};
}

Vec3<float> normalized(Vec3<float> v) {
    const float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len < 1e-8f)
        return {0.0f, 1.0f, 0.0f};
    return {v.x / len, v.y / len, v.z / len};
}

float dot3(const Vec3<float>& a, const Vec3<float>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3<float> cross3(const Vec3<float>& a, const Vec3<float>& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

constexpr float kDegToRad = std::numbers::pi_v<float> / 180.0f;

std::string member(const std::string& base, const std::string& name) {
    return base + "." + name;
}

} // namespace

AtmosphericScattering::LutPass AtmosphericScattering::makeLut(const char* shader, const int width, const int height) {
    LutPass lut;
    lut.width = width;
    lut.height = height;
    lut.target = std::make_unique<GLFramebuffer>(width, height, GLFramebuffer::Depth::None,
                                                 GLTexture::Format::ColorHDR);
    lut.pass = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                 generatedShader(std::string("atmospheric_scattering/") + shader));
    return lut;
}

AtmosphericScattering::AtmosphericScattering(AtmosphereProfile profile)
    : m_profile(std::move(profile)) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("atmospheric_scattering/composite.frag"));
    m_transmittance = makeLut("transmittance.frag", kTransmittanceWidth, kTransmittanceHeight);
    m_multiScatter = makeLut("multiscatter.frag", kMultiScatterSize, kMultiScatterSize);
    m_skyView = makeLut("skyview.frag", kSkyViewWidth, kSkyViewHeight);
    m_aerial = makeLut("aerial.frag", kAerialSlices * kAerialTile, kAerialTile);
}

void AtmosphericScattering::resize(const int width, const int height) {
    Effect::resize(width, height);
}

void AtmosphericScattering::render(const GLTexture& input) {
    if (!m_enabled)
        return;
    renderImpl(input, currentDrawFramebuffer());
}

void AtmosphericScattering::render(const GLTexture& input, GLFramebuffer& target) {
    if (!m_enabled)
        return;
    renderImpl(input, target.handle());
}


MediumComponent AtmosphericScattering::scaledComponent(const std::size_t index) const {
    MediumComponent c = m_profile.components[index];

    float scale = m_profile.densityScale;
    if (index == AtmosphereProfile::kRayleigh) {
        c.scattering = {c.scattering.x * m_profile.rayleighTint.x,
                        c.scattering.y * m_profile.rayleighTint.y,
                        c.scattering.z * m_profile.rayleighTint.z};
    } else if (index == AtmosphereProfile::kMie) {
        scale *= m_profile.hazeAmount;
    } else if (index == AtmosphereProfile::kOzone) {
        scale *= m_profile.ozoneAmount;
    }

    // The planet-size rescale
    const float k = m_frame.mediumScale / std::max(m_profile.heightScale, 1e-3f);
    scale *= k;

    c.scattering = {c.scattering.x * scale, c.scattering.y * scale, c.scattering.z * scale};
    c.absorption = {c.absorption.x * scale, c.absorption.y * scale, c.absorption.z * scale};
    for (DensityLayer& layer : c.density) {
        layer.width /= k;
        layer.expScale *= k;
        layer.linearTerm *= k;
    }
    return c;
}

std::vector<float> AtmosphericScattering::mediumSnapshot() const {
    std::vector<float> snap;
    snap.reserve(8 + 4 * 20);
    snap.push_back(m_frame.groundRadius);
    snap.push_back(m_frame.topRadius);
    snap.push_back(m_profile.multiScatteringFactor);
    snap.push_back(m_profile.groundAlbedo.x);
    snap.push_back(m_profile.groundAlbedo.y);
    snap.push_back(m_profile.groundAlbedo.z);
    snap.push_back(m_profile.heightScale);
    snap.push_back(static_cast<float>(m_quality.transmittanceSteps));
    snap.push_back(static_cast<float>(m_quality.multiScatterDirections * 1000 + m_quality.multiScatterSteps));
    for (std::size_t i = 0; i < 4; ++i) {
        const MediumComponent c = scaledComponent(i);
        for (const float v : {c.scattering.x, c.scattering.y, c.scattering.z,
                              c.absorption.x, c.absorption.y, c.absorption.z,
                              static_cast<float>(c.phase), c.anisotropy})
            snap.push_back(v);
        for (const DensityLayer& l : c.density)
            for (const float v : {l.width, l.expTerm, l.expScale, l.linearTerm, l.constantTerm})
                snap.push_back(v);
    }
    return snap;
}

void AtmosphericScattering::computeFrame() {
    Frame f;
    f.groundRadius = m_profile.groundRadiusKm;

    if (m_scaleMode == ScaleMode::Physical || m_scaleMode == ScaleMode::Sky) {
        f.kmPerUnit = static_cast<float>(1.0 / m_unitsPerKilometer);
        f.topRadius = f.groundRadius + m_profile.atmosphereHeightKm * std::max(m_profile.heightScale, 1e-3f);
        f.mediumScale = 1.0f;
    } else {
        const double kmPerUnit = m_profile.groundRadiusKm / m_planetRadiusUnits;
        const double heightKm = m_atmosphereHeightUnits * kmPerUnit;
        f.kmPerUnit = static_cast<float>(kmPerUnit);
        f.topRadius = f.groundRadius + static_cast<float>(heightKm);
        f.mediumScale = static_cast<float>(m_profile.atmosphereHeightKm / heightKm);
    }

    if (m_scaleMode == ScaleMode::Sky) {
        /* The planet sits right under the camera, so the camera is always at the top of its
         * centre along the up axis. */
        Vec3<float> up{0.0f}, north{0.0f}, east{0.0f};
        worldBasis(up, north, east);
        const Vec3<double> cam{m_camera.position.x, m_camera.position.y, m_camera.position.z};
        const double heightUnits = cam.x * up.x + cam.y * up.y + cam.z * up.z - m_seaLevel;
        const double radius = f.groundRadius + std::max(heightUnits * f.kmPerUnit, 0.001);
        f.cameraPosKm = {static_cast<float>(up.x * radius), static_cast<float>(up.y * radius),
                         static_cast<float>(up.z * radius)};
        f.inside = radius < f.topRadius;
    } else {
        // Camera relative to the planet, in doubles until it is small enough for floats.
        const double dx = (static_cast<double>(m_camera.position.x) - m_planetCenter.x) * f.kmPerUnit;
        const double dy = (static_cast<double>(m_camera.position.y) - m_planetCenter.y) * f.kmPerUnit;
        const double dz = (static_cast<double>(m_camera.position.z) - m_planetCenter.z) * f.kmPerUnit;
        f.cameraPosKm = {static_cast<float>(dx), static_cast<float>(dy), static_cast<float>(dz)};
        f.inside = std::sqrt(dx * dx + dy * dy + dz * dz) < f.topRadius;
    }

    f.sunDirection = normalized(m_sunAngles ? directionFromAngles(m_sunAngles->first, m_sunAngles->second)
                                            : m_sunDirection);
    f.invProjection = m_camera.projection.inverse();

    /* View -> world is the inverse view matrix. The planet frame is the world frame, so
     * that is also view -> planet once the translation is dropped (directions only). */
    f.viewToPlanet = m_camera.view.inverse();
    f.viewToPlanet.m[12] = 0.0f;
    f.viewToPlanet.m[13] = 0.0f;
    f.viewToPlanet.m[14] = 0.0f;

    m_frame = f;
}

void AtmosphericScattering::uploadAtmosphere(GLPostProcessor& pass) const {
    const std::string a = "u_atm";
    pass.trySetUniform(member(a, "groundRadius"), m_frame.groundRadius);
    pass.trySetUniform(member(a, "topRadius"), m_frame.topRadius);
    pass.trySetUniform(member(a, "multiScatteringFactor"), m_profile.multiScatteringFactor);
    pass.trySetUniform(member(a, "sunAngularRadius"),
                       m_sunAngularRadiusDeg * std::numbers::pi_v<float> / 180.0f);
    pass.trySetUniform(member(a, "groundAlbedo"), xyz0(m_profile.groundAlbedo));
    pass.trySetUniform(member(a, "sunIlluminance"), xyz0(m_sunIlluminance));
    pass.trySetUniform(member(a, "sunDirection"), xyz0(m_frame.sunDirection));
    pass.trySetUniform(member(a, "cameraPos"), xyz0(m_frame.cameraPosKm));

    for (std::size_t i = 0; i < 4; ++i) {
        const MediumComponent c = scaledComponent(i);
        const std::string base = a + ".components[" + std::to_string(i) + "]";
        pass.trySetUniform(member(base, "scattering"), xyz0(c.scattering));
        pass.trySetUniform(member(base, "absorption"), xyz0(c.absorption));
        pass.trySetUniform(member(base, "phaseId"), static_cast<int>(c.phase));
        pass.trySetUniform(member(base, "phaseParam"), c.anisotropy);
        for (std::size_t l = 0; l < 2; ++l) {
            const std::string layer = base + ".layers[" + std::to_string(l) + "]";
            pass.trySetUniform(member(layer, "width"), c.density[l].width);
            pass.trySetUniform(member(layer, "expTerm"), c.density[l].expTerm);
            pass.trySetUniform(member(layer, "expScale"), c.density[l].expScale);
            pass.trySetUniform(member(layer, "linearTerm"), c.density[l].linearTerm);
            pass.trySetUniform(member(layer, "constantTerm"), c.density[l].constantTerm);
        }
    }

    const auto lutSize = [](const int w, const int h) {
        return Vec4(static_cast<float>(w), static_cast<float>(h), 1.0f / static_cast<float>(w), 1.0f / static_cast<float>(h));
    };
    pass.trySetUniform(member(a, "transmittanceLutSize"), lutSize(kTransmittanceWidth, kTransmittanceHeight));
    pass.trySetUniform(member(a, "multiScatterLutSize"), lutSize(kMultiScatterSize, kMultiScatterSize));
    pass.trySetUniform(member(a, "skyViewLutSize"), lutSize(kSkyViewWidth, kSkyViewHeight));
    // Aerial distance is authored in profile km, a rescaled planet shrinks it with the air.
    pass.trySetUniform(member(a, "aerial"), Vec4(static_cast<float>(kAerialSlices), static_cast<float>(kAerialTile),
                                                 m_profile.aerialMaxDistanceKm / m_frame.mediumScale, 0.0f));
    pass.trySetUniform(member(a, "sunDisk"), Vec4(m_sunDiskIntensity, m_sunLimbDarkening, 0.0f, 0.0f));
}

void AtmosphericScattering::uploadView(GLPostProcessor& pass) const {
    const std::string v = "u_view";
    pass.trySetUniform(member(v, "invProjection"), m_frame.invProjection);
    pass.trySetUniform(member(v, "viewToPlanet"), m_frame.viewToPlanet);
    pass.trySetUniform(member(v, "depth"), Vec4(
        m_depthConvention.range == DepthConvention::Range::NegOneToOne ? 0 : 1,
        m_depthConvention.reversed ? 1 : 0,
        m_depthConvention.flipY ? 1 : 0,
        0));
    pass.trySetUniform(member(v, "scale"), Vec4(m_frame.kmPerUnit, std::max(m_aerialDistanceScale, 0.0f),
                                                m_scaleMode == ScaleMode::Sky ? 1.0f : 0.0f, 0.0f));
}

void AtmosphericScattering::updateUniforms() {
    uploadAtmosphere(*m_processor);
    uploadView(*m_processor);
    m_processor->setUniform("u_resolution", m_resolution);
    m_processor->setUniform("u_spaceSamples", m_quality.spaceSamples);
    m_processor->setUniform("u_referenceSamples", m_quality.referenceSamples);
    m_processor->setUniform("u_debugMode", static_cast<int>(m_debugMode));
    m_processor->setUniform("u_outputHdr", m_outputHdr.value_or(isPipelineHdr()) ? 1 : 0);
    m_processor->setUniform("u_sceneIsLinear", m_sceneLinear ? 1 : 0);
    m_processor->setUniform("u_jitter", m_quality.jitter ? 1 : 0);
    m_processor->setUniform("u_exposure", m_exposure);
    m_processor->setUniform("u_debugExposure", m_debugExposure);
    m_processor->setUniform("u_virtualGround", m_virtualGround.value_or(m_scaleMode != ScaleMode::Sky) ? 1 : 0);
}

void AtmosphericScattering::runLutPass(LutPass& lut, const GLTexture& dummyInput) {
    GLC(glViewport(0, 0, lut.width, lut.height));
    lut.target->bind();
    lut.pass->render(dummyInput);
}

void AtmosphericScattering::renderImpl(const GLTexture& input, const uint32_t target) {
    /* Every pass here writes data, not colours to mix. The aerial table keeps its
     * transmittance in alpha. A host that leaves blending on (raylib does) would blend
     * each pass by that alpha and wipe out the far haze. The guard puts the host's state
     * back afterwards. */
    const GLStateGuard guard;
    GLC(glDisable(GL_BLEND));

    computeFrame();

    // Medium LUTs, only when what they depend on changed.
    if (std::vector<float> snap = mediumSnapshot(); snap != m_uploadedMedium) {
        m_uploadedMedium = std::move(snap);

        uploadAtmosphere(*m_transmittance.pass);
        m_transmittance.pass->setUniform("u_steps", m_quality.transmittanceSteps);
        runLutPass(m_transmittance, input);

        uploadAtmosphere(*m_multiScatter.pass);
        m_multiScatter.pass->setUniform("u_directions", m_quality.multiScatterDirections);
        m_multiScatter.pass->setUniform("u_steps", m_quality.multiScatterSteps);
        m_multiScatter.pass->addInputTexture("u_transmittanceLut", m_transmittance.target->color(), kUnitTransmittance);
        runLutPass(m_multiScatter, input);
    }

    // View LUTs, every frame, only useful from inside the atmosphere.
    if (m_frame.inside && m_debugMode != DebugMode::Reference) {
        uploadAtmosphere(*m_skyView.pass);
        m_skyView.pass->setUniform("u_samples", m_quality.skyViewSamples);
        m_skyView.pass->addInputTexture("u_transmittanceLut", m_transmittance.target->color(), kUnitTransmittance);
        m_skyView.pass->addInputTexture("u_multiScatterLut", m_multiScatter.target->color(), kUnitMultiScatter);
        runLutPass(m_skyView, input);

        uploadAtmosphere(*m_aerial.pass);
        uploadView(*m_aerial.pass);
        m_aerial.pass->setUniform("u_samples", m_quality.aerialSamples);
        m_aerial.pass->addInputTexture("u_transmittanceLut", m_transmittance.target->color(), kUnitTransmittance);
        m_aerial.pass->addInputTexture("u_multiScatterLut", m_multiScatter.target->color(), kUnitMultiScatter);
        runLutPass(m_aerial, input);
    }

    // Composite at full resolution into the caller's target.
    GLC(glViewport(0, 0, static_cast<int>(m_resolution.x), static_cast<int>(m_resolution.y)));
    bindFramebuffer(target);

    updateUniforms();
    m_processor->addInputTexture("u_depthTexture", *m_depth, kUnitDepth);
    m_processor->addInputTexture("u_transmittanceLut", m_transmittance.target->color(), kUnitTransmittance);
    m_processor->addInputTexture("u_multiScatterLut", m_multiScatter.target->color(), kUnitMultiScatter);
    m_processor->addInputTexture("u_skyViewLut", m_skyView.target->color(), kUnitSkyView);
    m_processor->addInputTexture("u_aerialLut", m_aerial.target->color(), kUnitAerial);
    m_processor->render(input);
}

AtmosphericScattering& AtmosphericScattering::withCamera(const Camera& camera) {
    m_camera = camera;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withDepthConvention(const DepthConvention convention) {
    m_depthConvention = convention;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withPlanet(const Vec3<double> center, const double radius,
                                                         const double atmosphereHeight) {
    m_scaleMode = ScaleMode::Normalized;
    m_planetCenter = center;
    m_planetRadiusUnits = radius;
    m_atmosphereHeightUnits = atmosphereHeight;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withPhysicalPlanet(const Vec3<double> center,
                                                                 const double unitsPerKilometer) {
    m_scaleMode = ScaleMode::Physical;
    m_planetCenter = center;
    m_unitsPerKilometer = unitsPerKilometer;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withSkyMode(const double seaLevel, const double unitsPerKilometer) {
    m_scaleMode = ScaleMode::Sky;
    m_seaLevel = seaLevel;
    m_unitsPerKilometer = unitsPerKilometer;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withWorldAxes(const Vec3<float> up, const Vec3<float> north) {
    m_worldUp = up;
    m_worldNorth = north;
    if (m_sunAngles)
        m_sunDirection = directionFromAngles(m_sunAngles->first, m_sunAngles->second);
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withAerialDistanceScale(const float scale) {
    m_aerialDistanceScale = scale;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withVirtualGround(const bool visible) {
    m_virtualGround = visible;
    return *this;
}

void AtmosphericScattering::worldBasis(Vec3<float>& up, Vec3<float>& north, Vec3<float>& east) const {
    up = normalized(m_worldUp);
    // Only the horizontal part of north counts, fall back on any horizontal direction.
    const float along = dot3(m_worldNorth, up);
    north = {m_worldNorth.x - up.x * along, m_worldNorth.y - up.y * along, m_worldNorth.z - up.z * along};
    if (dot3(north, north) < 1e-8f)
        north = std::fabs(up.x) < 0.9f ? cross3(up, Vec3(1.0f, 0.0f, 0.0f)) : cross3(up, Vec3(0.0f, 0.0f, 1.0f));
    north = normalized(north);
    // Compass order, looking down from above, east is a quarter turn clockwise from north.
    east = cross3(north, up);
}

Vec3<float> AtmosphericScattering::directionFromAngles(const float elevationDegrees, const float azimuthDegrees) const {
    Vec3<float> up{0.0f}, north{0.0f}, east{0.0f};
    worldBasis(up, north, east);
    const float el = elevationDegrees * kDegToRad;
    const float az = azimuthDegrees * kDegToRad;
    const float h = std::cos(el);
    const float n = std::cos(az) * h;
    const float e = std::sin(az) * h;
    const float u = std::sin(el);
    return {north.x * n + east.x * e + up.x * u,
            north.y * n + east.y * e + up.y * u,
            north.z * n + east.z * e + up.z * u};
}

AtmosphericScattering& AtmosphericScattering::withSunDirection(const Vec3<float> toSun) {
    m_sunDirection = toSun;
    m_sunAngles.reset();
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withSunAngles(const float elevationDegrees, const float azimuthDegrees) {
    m_sunAngles = std::make_pair(elevationDegrees, azimuthDegrees);
    m_sunDirection = directionFromAngles(elevationDegrees, azimuthDegrees);
    return *this;
}

float AtmosphericScattering::getSunElevation() const {
    if (m_sunAngles)
        return m_sunAngles->first;
    Vec3<float> up{0.0f}, north{0.0f}, east{0.0f};
    worldBasis(up, north, east);
    return std::asin(std::clamp(dot3(normalized(m_sunDirection), up), -1.0f, 1.0f)) / kDegToRad;
}

float AtmosphericScattering::getSunAzimuth() const {
    if (m_sunAngles)
        return m_sunAngles->second;
    Vec3<float> up{0.0f}, north{0.0f}, east{0.0f};
    worldBasis(up, north, east);
    const Vec3<float> d = normalized(m_sunDirection);
    const float azimuth = std::atan2(dot3(d, east), dot3(d, north)) / kDegToRad;
    return azimuth < 0.0f ? azimuth + 360.0f : azimuth;
}

AtmosphericScattering& AtmosphericScattering::withSunIlluminance(const Vec3<float> illuminance) {
    m_sunIlluminance = illuminance;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withSunDisk(const float angularRadiusDegrees, const float intensity,
                                                          const float limbDarkening) {
    m_sunAngularRadiusDeg = angularRadiusDegrees;
    m_sunDiskIntensity = intensity;
    m_sunLimbDarkening = limbDarkening;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withProfile(const AtmosphereProfile& profile) {
    m_profile = profile;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withExposure(const float exposure) {
    m_exposure = exposure;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withSceneLinear(const bool linear) {
    m_sceneLinear = linear;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withOutputHdr(const std::optional<bool> hdr) {
    m_outputHdr = hdr;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withQuality(const Quality quality) {
    m_quality = quality;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withDebugMode(const DebugMode mode) {
    m_debugMode = mode;
    return *this;
}

AtmosphericScattering& AtmosphericScattering::withDebugExposure(const float exposure) {
    m_debugExposure = exposure;
    return *this;
}
