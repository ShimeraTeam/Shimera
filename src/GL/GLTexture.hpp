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

#include <cstdint>

namespace shimera {

/**
 * A GL texture the core can sample from.
 * Here the core knows only GLuint, and extraction happens once, in the host lib.
 * There is deliberately no unbind()
 */
class GLTexture {
    public:
        enum class Format { Color, Depth };

        /* adopt a texture someone else created (the host library), this is the most widely used method
         * Shimera never deletes it. This is the normal case. */
        GLTexture(uint32_t handle, int width, int height) noexcept;

        // create and own a texture (pipeline intermediates)
        GLTexture(int width, int height, Format format = Format::Color);

        ~GLTexture();

        GLTexture(GLTexture&& other) noexcept;
        GLTexture& operator=(GLTexture&& other) noexcept;
        GLTexture(const GLTexture&) = delete;
        GLTexture& operator=(const GLTexture&) = delete;

        void bind(int slot = 0) const;

        [[nodiscard]] uint32_t getHandle() const { return m_handle; }
        [[nodiscard]] int getWidth() const { return m_width; }
        [[nodiscard]] int getHeight() const { return m_height; }
        [[nodiscard]] bool owned() const { return m_owned; }

    private:
        void destroy() noexcept;

        uint32_t m_handle = 0;
        int m_width = 0;
        int m_height = 0;
        bool m_owned = false;
};

}
