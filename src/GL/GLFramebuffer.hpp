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
#include <optional>

#include "GLTexture.hpp"
#include "common/Color.inl"

namespace shimera {

/**
 * The framebuffer currently being drawn into, as a raw GL name.
 * Callers that need to restore a target they did not create use these two.
 */
[[nodiscard]] uint32_t currentDrawFramebuffer();
void bindFramebuffer(uint32_t handle);

// Clears just the depth (and stencil) of whatever is bound, leaving colour alone.
void clearDepth();

/**
 * An offscreen render target, one colour texture, plus depth as either a samplable
 * texture, a plain renderbuffer, or nothing at all.
 *
 * `colorFormat` picks LDR (RGBA8) or HDR (RGBA16F) storage. `Depth::None` is for
 * targets that only ever receive fullscreen passes, such as lookup tables, where a
 * depth attachment would be memory spent on nothing.
 */
class GLFramebuffer {
    public:
        enum class Depth { None, Renderbuffer, Texture };

        GLFramebuffer(int width, int height, bool samplableDepth = false,
                      GLTexture::Format colorFormat = GLTexture::Format::Color);
        GLFramebuffer(int width, int height, Depth depth, GLTexture::Format colorFormat);
        ~GLFramebuffer();

        GLFramebuffer(const GLFramebuffer&) = delete;
        GLFramebuffer& operator=(const GLFramebuffer&) = delete;
        GLFramebuffer(GLFramebuffer&&) = delete;
        GLFramebuffer& operator=(GLFramebuffer&&) = delete;

        void bind() const;
        static void clear(Color color = Color::black());
        void resize(int width, int height);

        [[nodiscard]] GLTexture& color() { return *m_color; }
        [[nodiscard]] const GLTexture& color() const { return *m_color; }
        /* nullptr unless this framebuffer was built with samplableDepth.
        * could be a const method, but i'll wait the end of the POC before doing that */
        [[nodiscard]] GLTexture* depth() { return m_depth ? &*m_depth : nullptr; }

        [[nodiscard]] uint32_t handle() const { return m_fbo; }
        [[nodiscard]] int width() const { return m_width; }
        [[nodiscard]] int height() const { return m_height; }
        [[nodiscard]] GLTexture::Format colorFormat() const { return m_colorFormat; }

    private:
        uint32_t m_fbo = 0;
        uint32_t m_rbo = 0;
        std::optional<GLTexture> m_color;
        std::optional<GLTexture> m_depth;
        int m_width = 0;
        int m_height = 0;
        Depth m_depthMode = Depth::Renderbuffer;
        GLTexture::Format m_colorFormat = GLTexture::Format::Color;
};

}
