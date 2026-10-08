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

#include "Effect.hpp"

namespace shimera {

/**
 * render(input, target) does not unbind the target afterwards (unlike the beta). The
 * pipeline decides what is bound next, so unbinding here would just be a guess that
 * the default framebuffer is 0.
 */
template<typename Derived>
class ShaderEffect : public Effect {
    public:
        void render(const GLTexture& input) override {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->updateUniforms();
            m_processor->render(input);
        }

        void render(const GLTexture& input, GLFramebuffer& target) override {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->updateUniforms();
            target.bind();
            m_processor->render(input);
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    private:
        ShaderEffect() = default;
        friend Derived;
};

}
