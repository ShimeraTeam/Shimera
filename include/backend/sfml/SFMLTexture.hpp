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

#ifndef SHIMERA_SFMLTEXTURE_HPP
#define SHIMERA_SFMLTEXTURE_HPP

#include <SFML/Graphics/Texture.hpp>
#include "backend/ITexture.hpp"

namespace shimera {

/**
 * SFML Texture wrapper implementing ITexture interface
 * Wraps an existing sf::Texture to help with abstraction
 */
class SFMLTexture final : public ITexture {
public:
    explicit SFMLTexture(const sf::Texture& texture);
    ~SFMLTexture() override = default;

    void bind(int slot = 0) override;
    void unbind() override;
    [[nodiscard]] unsigned int getNativeHandle() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;

private:
    const sf::Texture& m_texture;
};

}

#endif //SHIMERA_SFMLTEXTURE_HPP
