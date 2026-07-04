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
#include "backend/raylib/RaylibTexture.hpp"

using shimera::RaylibTexture;

RaylibTexture::RaylibTexture(const Texture& texture) : m_texture(texture) {}

void RaylibTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_texture.id));
}

void RaylibTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int RaylibTexture::getNativeHandle() const {
    return m_texture.id;
}

int RaylibTexture::getWidth() const {
    return m_texture.width;
}

int RaylibTexture::getHeight() const {
    return m_texture.height;
}
