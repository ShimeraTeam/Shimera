#include <SFML/Graphics.hpp>
#include <optional>
#include "Shader.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 400}), sf::String(L"SFML3 - Formes géométriques"));

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(175.f - 80.f, 200.f - 80.f));

    sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(sf::Vector2f(400.f - 80.f, 200.f - 80.f));

    sf::CircleShape triangle(105.f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(625.f - 105.f, 200.f - 80.f));
    //Shader shader("../res/shaders/BasicShader.vert", "../res/shaders/BasicShader.frag");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(circle);
        window.draw(rectangle);
        window.draw(triangle);

        window.display();
    }

    return 0;
}