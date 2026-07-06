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

#include "backend/opengl/OpenGLTexture.hpp"

#include <GL/glew.h>
#include <glUtils.h>
#include "backend/opengl/OpenGLTexture.hpp"

using shimera::OpenGLTexture;

OpenGLTexture::OpenGLTexture(const int width, const int height, Format format)
    : m_textureId(0), m_width(width), m_height(height) {
    GLC(glGenTextures(1, &m_textureId));
    GLC(glBindTexture(GL_TEXTURE_2D, m_textureId));

    if (format == Format::Depth) {
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
        0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    } else {
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLTexture::~OpenGLTexture() {
    if (m_textureId != 0) {
        GLC(glDeleteTextures(1, &m_textureId));
    }
}

void OpenGLTexture::bind(int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_textureId));
}

void OpenGLTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int OpenGLTexture::getNativeHandle() const {
    return m_textureId;
}

int OpenGLTexture::getWidth() const {
    return m_width;
}

int OpenGLTexture::getHeight() const {
    return m_height;
}
