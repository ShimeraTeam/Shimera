// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
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

#include "effects/AtmosphericScatteringEffect.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "converts/GlmConvert.hpp"

using shimera::AtmosphericScatteringEffect;
using shimera::IBackend;
using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::ITexture;
using shimera::Mat4;
using shimera::Vec3;
using shimera::toGlmVec3;
using shimera::toShimMat4;

AtmosphericScatteringEffect::AtmosphericScatteringEffect(IBackend* backend) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/atmospheric_scattering.frag"
        )
    );
}

void AtmosphericScatteringEffect::updateUniforms() {
    const glm::mat4 proj = glm::perspective(glm::radians(m_fovYDegrees), m_aspect, m_uNear, m_uFar);
    const glm::mat4 view = glm::lookAt(
        toGlmVec3(m_uCameraPos), toGlmVec3(m_uCameraTarget), toGlmVec3(m_uCameraUp));

    // avoid division by 0
    m_uOpticalDepthSamples = std::max(2, m_uOpticalDepthSamples);
    m_uInScatteringPoints  = std::max(2, m_uInScatteringPoints);

    m_processor->setUniform("u_cameraInverseProjection", toShimMat4(glm::inverse(proj)));
    m_processor->setUniform("u_cameraWorldMatrix", toShimMat4(glm::inverse(view)));

    m_processor->setUniform("u_cameraNear", m_uNear);
    m_processor->setUniform("u_cameraFar", m_uFar);
    m_processor->setUniform("u_cameraPos", m_uCameraPos);

    const glm::vec3 sun = glm::normalize(toGlmVec3(m_uLightDir));
    m_processor->setUniform("u_lightDir", Vec3(sun.x, sun.y, sun.z));

    m_processor->setUniform("u_planetCenter", m_uPlanetCenter);
    m_processor->setUniform("u_planetRadius", m_uPlanetRadius);
    m_processor->setUniform("u_atmosphereRadius", m_uAtmosphereRadius);
    m_processor->setUniform("u_densityFalloff", m_uDensityFalloff);
    m_processor->setUniform("u_scatterCoefficients", m_uScatterCoefficients);

    m_processor->setUniform("u_opticalDepthSamples", m_uOpticalDepthSamples);
    m_processor->setUniform("u_inScatteringPoints", m_uInScatteringPoints);
}

bool AtmosphericScatteringEffect::isDepthNeeded() const {
    return true;
}

void AtmosphericScatteringEffect::setDepthTexture(ITexture& depth) {
    m_depth = &depth;
}

void AtmosphericScatteringEffect::render(ITexture& scene) {
    if (!m_enabled) return;
    updateUniforms();
    if (m_depth != nullptr)
        m_processor->addInputTexture("u_depthTexture", *m_depth, 1);
    m_processor->render(scene);
}

void AtmosphericScatteringEffect::render(ITexture& scene, IFrameBuffer& target) {
    if (!m_enabled) return;
    updateUniforms();
    if (m_depth != nullptr)
        m_processor->addInputTexture("u_depthTexture", *m_depth, 1);
    target.bind();
    m_processor->render(scene);
    target.unbind();
}

AtmosphericScatteringEffect& AtmosphericScatteringEffect::withCamera(
        const Vec3<float> pos, const Vec3<float> target, const float fovY,
        const float aspect,const float near, const float far) {
    m_uCameraPos = pos;
    m_uCameraTarget = target;
    m_fovYDegrees = fovY;
    m_aspect = aspect;
    m_uNear = near;
    m_uFar = far;
    return *this;
}

AtmosphericScatteringEffect& AtmosphericScatteringEffect::withSun(const Vec3<float> dir) {
    m_uLightDir = dir;
    return *this;
}

AtmosphericScatteringEffect& AtmosphericScatteringEffect::withPlanet(
        const Vec3<float> center, const float radius, const float atmosphereRadius) {
    m_uPlanetCenter = center;
    m_uPlanetRadius = radius;
    m_uAtmosphereRadius = atmosphereRadius;
    return *this;
}

AtmosphericScatteringEffect& AtmosphericScatteringEffect::withQuality(
        const int opticalDepthSamples, const int inScatteringPoints) {
    m_uOpticalDepthSamples = std::max(2, opticalDepthSamples);
    m_uInScatteringPoints  = std::max(2, inScatteringPoints);
    return *this;
}

std::string AtmosphericScatteringEffect::getName() const { 
    return "AtmosphericScattering"; 
}
