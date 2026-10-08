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

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <optional>

#include "hosts/IHostTarget.hpp"

namespace shimera {

/**
 * Host target for SFML 3.
 *
 * This is the entire SFML integration. Compare the beta, 5 files, ~470 lines, of which
 * ~400 were OpenGL code pasted from the other two "backends".
 *
 * sf::RenderTexture carries its own GL context, so after drawing the scene SOMETHING
 * has to make the window current again or every later GL call lands in the wrong context.
 * Only the user knows which window, so the target is told once at construction and end() handles it.
 * That keeps the per-frame API identical to the other hosts.
 *
 * No getDepth() override, sf::RenderTexture can carry a depth buffer via ContextSettings
 * but will not hand it over as a samplable texture. To see if that causes problems in the future.
 */
class SfmlTarget final : public IHostTarget {
    public:
        SfmlTarget(sf::RenderWindow& window, int width, int height);

        void begin() override;
        void end() override;

        [[nodiscard]] GLTexture& getColor() override { return *m_color; }

        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override { return m_width; }
        [[nodiscard]] int getHeight() const override { return m_height; }

    protected:
        void* nativeHandle() override { return &m_renderTexture; }

    private:
        sf::RenderWindow* m_window;
        sf::RenderTexture m_renderTexture;
        std::optional<GLTexture> m_color;
        int m_width = 0;
        int m_height = 0;
};

}
