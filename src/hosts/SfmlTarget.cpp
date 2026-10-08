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

#include "SfmlTarget.hpp"

#include "GL/GLFramebuffer.hpp"

#include <stdexcept>
#include <string>

using shimera::SfmlTarget;

SfmlTarget::SfmlTarget(sf::RenderWindow& window, const int width, const int height)
    : m_window(&window) {
    SfmlTarget::resize(width, height);
}

void SfmlTarget::begin() {
    if (!m_renderTexture.setActive(true))
        throw std::runtime_error("shimera: SfmlTarget::begin() could not activate the render texture");

    /* SFML's clear() only touches colour, so without this a material's depth test reads
     * whatever was in the buffer last frame and the mesh either vanishes or self-occludes. */
    clearDepth();
}

void SfmlTarget::end() {
    /* display() resolves the texture and is REQUIRED before sampling it. This is SFML's
     * equivalent of raylib's batch flush. */
    m_renderTexture.display();

    /* Then hand the context back to the window. NOT setActive(false), that leaves no
     * context current at all and the next GL call segfaults.
     * Leaving the render texture current instead is quieter but just as wrong, the
     * effect pass then renders into the scene texture in the wrong context.
     * So the fix is for end() to hand the context back to the window (GLStateGuard purpose). */
    if (!m_window->setActive(true))
        throw std::runtime_error("shimera: SfmlTarget::end() could not reactivate the window");
}

void SfmlTarget::resize(const int width, const int height) {
    m_width = width;
    m_height = height;

    /* Ask for a depth buffer explicitly. SFML defaults to none, which costs nothing for
     * post-processing */
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    if (!m_renderTexture.resize({static_cast<unsigned>(width), static_cast<unsigned>(height)}, settings))
        throw std::runtime_error("shimera: SfmlTarget could not resize to "
            + std::to_string(width) + "x" + std::to_string(height));

    /* SFML may hand back a different texture after a resize, so reassign rather
    than assuming that the handle is correct. */
    m_color.emplace(m_renderTexture.getTexture().getNativeHandle(), width, height);
}
