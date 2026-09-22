#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/* A standalone SFML 3 example */
class SfmlMain {
public:
    SfmlMain(const char* title, unsigned int width, unsigned int height);

    SfmlMain(const SfmlMain&) = delete;
    SfmlMain& operator=(const SfmlMain&) = delete;

    void run();

private:
    void handleEvents();
    void draw();
    void drawBackground();

    sf::RenderWindow m_window;

    /* m_background is declared first on purpose: sf::Sprite has no default constructor
     * in SFML 3, it binds the texture at construction and keeps a reference to it. */
    sf::Texture m_background;
    sf::Sprite m_backgroundSprite;

    sf::RectangleShape m_square;
    sf::CircleShape m_circle;
};
