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

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include <optional>
#include <iostream>

#include <hosts/SfmlTarget.hpp>
#include <shimera.hpp>

class SfmlMain {
public:
    SfmlMain(const char* title, unsigned int width, unsigned int height);

    SfmlMain(const SfmlMain&) = delete;
    SfmlMain& operator=(const SfmlMain&) = delete;

    void initShimera();
    void run();

private:
    void handleEvents();
    void draw();

    void drawBackground(sf::RenderTarget& target);

    sf::RenderWindow m_window;

    std::optional<shimera::Context> m_context;
    std::optional<shimera::SfmlTarget> m_target;
    std::optional<shimera::EffectPipeline> m_pipeline;

    sf::Texture m_background;
    sf::Sprite m_backgroundSprite;

    sf::RectangleShape m_square;
    sf::CircleShape m_circle;

    sf::Clock m_clock;
};
