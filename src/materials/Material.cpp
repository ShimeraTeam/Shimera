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

#include "Material.hpp"

#include <GL/glew.h>

#include "GL/GLError.hpp"

using shimera::Material;

void Material::applySceneUniforms(const Camera& camera) const {
    m_shader->setUniform("u_model", m_transform.toMatrix());
    m_shader->setUniform("u_view", camera.view);
    m_shader->setUniform("u_projection", camera.projection);
    m_shader->setUniform("u_cameraPos", camera.position);
}

void Material::applyRenderState() const {
    if (m_depthTest) {
        GLC(glEnable(GL_DEPTH_TEST));
    } else {
        GLC(glDisable(GL_DEPTH_TEST));
    }

    if (m_cullBackFaces) {
        GLC(glEnable(GL_CULL_FACE));
        GLC(glCullFace(GL_BACK));
        GLC(glFrontFace(GL_CCW));
    } else {
        GLC(glDisable(GL_CULL_FACE));
    }

    if (m_blend == Blend::None) {
        GLC(glDisable(GL_BLEND));
        return;
    }

    GLC(glEnable(GL_BLEND));
    switch (m_blend) {
        case Blend::Alpha:
            GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            break;
        case Blend::Additive:
            GLC(glBlendFunc(GL_ONE, GL_ONE));
            break;
        case Blend::Premultiplied:
        default:
            GLC(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
            break;
    }
}
