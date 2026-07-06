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

#ifndef SHIMERA_MATERIALEFFECTBASE_HPP
#define SHIMERA_MATERIALEFFECTBASE_HPP

#include <memory>

#include "shimera_api.h"
#include "backend/IBackend.hpp"
#include "scene/Camera.hpp"

namespace shimera {

class SHIMERA_API MaterialEffectBase {
    public:
        virtual ~MaterialEffectBase() = default;

        void setEnabled(bool enabled);
        [[nodiscard]] bool isEnabled() const;

        void setTransform(const Vec3<float>& position,
                          const Vec3<float>& rotationEuler = Vec3(0.0f),
                          const Vec3<float>& scale = Vec3(1.0f));

    protected:
        IBackend* m_backend = nullptr;
        std::unique_ptr<IMaterial> m_material;
        Mat4 m_transform{{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}};
        bool m_enabled = true;
};

}

#endif //SHIMERA_MATERIALEFFECTBASE_HPP
