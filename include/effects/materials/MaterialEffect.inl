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

#ifndef SHIMERA_MATERIALEFFECT_INL
#define SHIMERA_MATERIALEFFECT_INL

#include "MaterialEffectBase.hpp"

namespace shimera {

template <typename Derived>
class MaterialEffect : public MaterialEffectBase {
    public:
        void render(IMesh& mesh, const Camera& camera) {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->uploadUniforms(*m_material);
            m_backend->renderMaterial(*m_material, mesh, camera, m_transform);
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    protected:
        MaterialEffect(IBackend* backend, const std::string& vert, const std::string& frag) {
            m_backend = backend;
            m_material.reset(backend->createMaterial(vert, frag));
        }
};

}

#endif //SHIMERA_MATERIALEFFECT_INL
