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

#include <GL/glew.h>
#include <glUtils.h>
#include <SFML/Graphics/Texture.hpp>
#include "backend/sfml/SFMLTexture.hpp"

using shimera::SFMLTexture;

SFMLTexture::SFMLTexture(const sf::Texture& texture) : m_texture(texture) {}

void SFMLTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_texture.getNativeHandle()));
}

void SFMLTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int SFMLTexture::getNativeHandle() const {
    return m_texture.getNativeHandle();
}

int SFMLTexture::getWidth() const {
    return static_cast<int>(m_texture.getSize().x);
}

int SFMLTexture::getHeight() const {
    return static_cast<int>(m_texture.getSize().y);
}
