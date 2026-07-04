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

#ifndef SHIMERA_OPENGLTEXTURE_HPP
#define SHIMERA_OPENGLTEXTURE_HPP

#include "backend/ITexture.hpp"

namespace shimera {

/**
 * OpenGL Texture implementation owning ITexture interface
 * Creates and manages an OpenGL texture resource with automatic cleanup
 */
class OpenGLTexture final : public ITexture {
    public:
        enum class Format {
            Color,
            Depth
        };

        explicit OpenGLTexture(int width, int height, Format format = Format::Color);
        ~OpenGLTexture() override;

        OpenGLTexture(const OpenGLTexture&) = delete;
        OpenGLTexture& operator=(const OpenGLTexture&) = delete;
        OpenGLTexture(OpenGLTexture&&) = delete;
        OpenGLTexture& operator=(OpenGLTexture&&) = delete;

        void bind(int slot = 0) override;
        void unbind() override;
        [[nodiscard]] unsigned int getNativeHandle() const override;
        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;

    private:
        unsigned int m_textureId;
        int m_width;
        int m_height;
        Format m_format;
};

}

#endif //SHIMERA_OPENGLTEXTURE_HPP
