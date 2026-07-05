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

#ifndef SHIMERA_COLORTINTEFFECT_HPP
#define SHIMERA_COLORTINTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

class SHIMERA_API ColortintEffect final : public ShaderEffect<ColortintEffect> {
    public:
        Vec3<float> m_uTint = Vec3<float>(0.0f);

        explicit ColortintEffect(IBackend *backend);

        ColortintEffect(IBackend *backend, Vec3<float> tint);

        void updateUniforms() override;

        ColortintEffect &withTint(const Vec3<float> &tint);

        std::string getName() const override;
};

}

#endif //SHIMERA_COLORTINTEFFECT_HPP