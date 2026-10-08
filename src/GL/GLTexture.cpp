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

#include "GLTexture.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

#include <utility>

using shimera::GLTexture;

GLTexture::GLTexture(const uint32_t handle, const int width, const int height) noexcept
    : m_handle(handle), m_width(width), m_height(height), m_owned(false) {}

GLTexture::GLTexture(const int width, const int height, const Format format)
    : m_width(width), m_height(height), m_owned(true) {
    // Hosts cache which texture they think is bound, guard those so we don't mess those up.
    const GLStateGuard guard;

    GLC(glGenTextures(1, &m_handle));
    GLC(glBindTexture(GL_TEXTURE_2D, m_handle));

    if (format == Format::Depth) {
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
            0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    } else {
        /* RGBA rather than the beta's RGB. effects composite with alpha, and an
        RGB attachment silently drops it.*/
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

GLTexture::~GLTexture() {
    destroy();
}

GLTexture::GLTexture(GLTexture&& other) noexcept
    : m_handle(other.m_handle), m_width(other.m_width),
      m_height(other.m_height), m_owned(other.m_owned) {
    other.m_handle = 0;
    other.m_owned = false;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept {
    if (this != &other) {
        destroy();
        m_handle = std::exchange(other.m_handle, 0);
        m_width = other.m_width;
        m_height = other.m_height;
        m_owned = std::exchange(other.m_owned, false);
    }
    return *this;
}

void GLTexture::destroy() noexcept {
    if (m_owned && m_handle != 0) {
        glDeleteTextures(1, &m_handle);
    }
    m_handle = 0;
    m_owned = false;
}

void GLTexture::bind(const int slot) const {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_handle));
}
