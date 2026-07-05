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

#ifndef SHIMERA_CONTRASTEFFECT_HPP
#define SHIMERA_CONTRASTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API ContrastEffect final : public ShaderEffect<ContrastEffect> {
    public:
        float m_uContrastFactor = 1.0f;

        explicit ContrastEffect(IBackend *backend);

        ContrastEffect(IBackend *backend, float u_contrastFactor);

        void updateUniforms() override;

        ContrastEffect &withContrastFactor(float u_contrastFactor);

        std::string getName() const override;
};

}

#endif //SHIMERA_CONTRASTEFFECT_HPP