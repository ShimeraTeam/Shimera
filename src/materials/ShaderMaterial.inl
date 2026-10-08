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

#include "GL/GLStateGuard.hpp"
#include "Material.hpp"

namespace shimera {

/**
 * CRTP helper, a material only has to write updateUniforms() and its parameters, exactly
 * like ShaderEffect does for the post-processing side.
 */
template<typename Derived>
class ShaderMaterial : public Material {
    public:
        // Draws into whatever framebuffer is bound, typically the user's scene target.
        void render(const GLMesh& mesh, const Camera& camera) {
            if (!m_enabled)
                return;

            const GLStateGuard guard;

            m_shader->bind();
            applySceneUniforms(camera);
            static_cast<Derived*>(this)->updateUniforms();
            applyRenderState();
            mesh.draw();
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    private:
        ShaderMaterial() = default;
        friend Derived;
};

}
