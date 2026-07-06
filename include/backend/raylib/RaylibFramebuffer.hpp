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

#ifndef SHIMERA_RAYLIBFRAMEBUFFER_HPP
#define SHIMERA_RAYLIBFRAMEBUFFER_HPP

#include <memory>
#include <raylib.h>
#include "backend/IFrameBuffer.hpp"
#include "RaylibTexture.hpp"

namespace shimera {

/**
 * Raylib Framebuffer implementation
 * Wraps around Raylib RenderTexture to provide framebuffer functionality
 */
class RaylibFramebuffer final : public IFrameBuffer {
public:
    RaylibFramebuffer(int width, int height, bool samplableDepth);
    ~RaylibFramebuffer() override = default;

    void bind() override;
    void unbind() override;
    void clear(Color color) override;
    ITexture& getTexture() override;
    ITexture& getDepthTexture() override;
    void resize(int width, int height) override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    void* getNativeRenderTarget() override;

private:
    RenderTexture2D m_renderTexture;
    std::unique_ptr<RaylibTexture> m_texture;
    std::unique_ptr<RaylibTexture> m_depthTexture;
    int m_width, m_height;
    bool m_samplableDepth;
};

}

#endif //SHIMERA_RAYLIBFRAMEBUFFER_HPP