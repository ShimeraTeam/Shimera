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

#ifndef SHIMERA_SHADEREFFECT_INL
#define SHIMERA_SHADEREFFECT_INL
#include "ShaderEffectBase.hpp"

namespace shimera {

/*
 * Templated shader effect class
 * Inherits from ShaderEffectBase and provides a convenient interface for creating specific shader effects
 * The Derived template parameter allows for type-safe access to the derived class's methods and members
 */
template<typename Derived>
class ShaderEffect : public ShaderEffectBase {
    public:
        void render(ITexture& texture) override {
            if (!m_enabled) {
                return;
            }
            static_cast<Derived *>(this)->updateUniforms();
            m_processor->render(texture);
        }

        void render(ITexture& texture, IFrameBuffer& target) override {
            if (!m_enabled) {
                return;
            }
            static_cast<Derived *>(this)->updateUniforms();
            target.bind();
            m_processor->render(texture);
            target.unbind();
        }

        Derived &with() {
            return *static_cast<Derived *>(this);
        }

    private:
        ShaderEffect() = default;
        friend Derived;
};

}

#endif //SHIMERA_SHADEREFFECT_INL