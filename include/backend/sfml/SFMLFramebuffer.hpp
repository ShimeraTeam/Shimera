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

#ifndef SHIMERA_SFMLFRAMEBUFFER_HPP
#define SHIMERA_SFMLFRAMEBUFFER_HPP

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "backend/IFrameBuffer.hpp"
#include "SFMLTexture.hpp"

namespace shimera {

/**
 * SFML Framebuffer implementation
 * Wraps around SFML RenderTexture to provide framebuffer functionality
 */
class SFMLFramebuffer final : public IFrameBuffer {
public:
    SFMLFramebuffer(int width, int height, bool samplableDepth = false);
    ~SFMLFramebuffer() override = default;

    void bind() override;
    void unbind() override;
    void clear(Color color) override;
    ITexture& getTexture() override;
    ITexture& getDepthTexture() override;
    void resize(int width, int height) override;
    int getWidth() const override;
    int getHeight() const override;
    void* getNativeRenderTarget() override;

private:
    sf::RenderTexture m_renderTexture;
    std::unique_ptr<SFMLTexture> m_texture;
    int m_width, m_height;
};

}

#endif //SHIMERA_SFMLFRAMEBUFFER_HPP