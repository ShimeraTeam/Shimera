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

#include "backend/sfml/SFMLFramebuffer.hpp"
#include <stdexcept>
#include <string>

using shimera::ITexture;
using shimera::SFMLFramebuffer;
using shimera::SFMLTexture;

SFMLFramebuffer::SFMLFramebuffer(const int width, const int height, bool samplableDepth)
    : m_width(width), m_height(height) {
    SFMLFramebuffer::resize(width, height);
}

void SFMLFramebuffer::bind() {
    m_renderTexture.setActive(true);
}

void SFMLFramebuffer::unbind() {
    m_renderTexture.display();
    m_renderTexture.setActive(false);
}

void SFMLFramebuffer::clear(shimera::Color color) {
    const sf::Color sfmlColor(
        static_cast<std::uint8_t>(color.m_r * 255),
        static_cast<std::uint8_t>(color.m_g * 255),
        static_cast<std::uint8_t>(color.m_b * 255),
        static_cast<std::uint8_t>(color.m_a * 255)
    );
    m_renderTexture.clear(sfmlColor);
}

ITexture& SFMLFramebuffer::getTexture() {
    return *m_texture;
}

ITexture& SFMLFramebuffer::getDepthTexture() {
    //TODO: In reality, sfml CAN have a depth buffer, but i don't need it rn, so maybe add it later?
    throw std::runtime_error("SFML cannot have a depth texture");
}

void SFMLFramebuffer::resize(int width, int height) {

    if (!sf::Context::getActiveContextId()) {
        throw std::runtime_error("No OpenGL context current");
    }

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Framebuffer size too small");
    }

    m_width = width;
    m_height = height;
    
    if (!m_renderTexture.resize({static_cast<unsigned>(width), static_cast<unsigned>(height)})) {
        throw std::runtime_error("Failed to resize SFML framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    m_texture = std::make_unique<SFMLTexture>(m_renderTexture.getTexture());
}

int SFMLFramebuffer::getWidth() const {
    return m_width;
}

int SFMLFramebuffer::getHeight() const {
    return m_height;
}

void* SFMLFramebuffer::getNativeRenderTarget() {
    return &m_renderTexture;
}
