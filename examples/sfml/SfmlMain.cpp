#include "SfmlMain.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <exception>
#include <iostream>

namespace {

constexpr unsigned int kWindowWidth = 960;
constexpr unsigned int kWindowHeight = 540;

constexpr const char* kBackgroundPath = "res/imgs/pattern_checkerboard_gray.png";

constexpr float kBackgroundScale = 0.75f;

constexpr float kSquareSize = 240.0f;
constexpr float kCircleRadius = 115.0f;

constexpr unsigned int kCirclePoints = 96;

}

SfmlMain::SfmlMain(const char* title, const unsigned int width, const unsigned int height)
    : m_window(sf::VideoMode({width, height}), title, sf::Style::Default),
      m_background(kBackgroundPath),
      m_backgroundSprite(m_background),
      m_square({kSquareSize, kSquareSize}),
      m_circle(kCircleRadius) {
    m_window.setVerticalSyncEnabled(true);

    m_background.setRepeated(true);
    m_backgroundSprite.setScale({kBackgroundScale, kBackgroundScale});

    m_square.setPosition({140.0f, 155.0f});
    m_square.setFillColor(sf::Color::Green);

    m_circle.setPointCount(kCirclePoints);
    m_circle.setPosition({555.0f, 160.0f});
    m_circle.setFillColor(sf::Color::Yellow);
}

void SfmlMain::initShimera() {
    const auto width = static_cast<int>(m_window.getSize().x);
    const auto height = static_cast<int>(m_window.getSize().y);

    m_context = shimera::Context::create();
    std::cout << shimera::Context::getRendererInfo() << '\n';

    m_target.emplace(m_window, width, height);
    m_pipeline.emplace(width, height);

    m_pipeline->add<shimera::Vignette>(1.0f, 0.45f, 0.35f)
        .add<shimera::Distortion>()
        .add<shimera::HDRBloom>();
}

void SfmlMain::run() {
    while (m_window.isOpen()) {
        handleEvents();
        draw();
    }
}

void SfmlMain::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape)
                m_window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            const auto width = static_cast<int>(resized->size.x);
            const auto height = static_cast<int>(resized->size.y);

            m_window.setView(sf::View(sf::FloatRect({0.0f, 0.0f}, sf::Vector2f(resized->size))));

            if (m_target)
                m_target->resize(width, height);
            if (m_pipeline)
                m_pipeline->resize(width, height);
        }
    }
}

void SfmlMain::draw() {
    sf::RenderTexture& scene = *m_target->native<sf::RenderTexture>();

    m_target->begin();
        scene.clear(sf::Color::Black);
        drawBackground(scene);
        scene.draw(m_square);
        scene.draw(m_circle);
    m_target->end();

    m_pipeline->get<shimera::Distortion>().withTime(m_clock.getElapsedTime().asSeconds());

    m_window.clear(sf::Color::Black);
    m_pipeline->render(*m_target);
    m_window.display();
}

void SfmlMain::drawBackground(sf::RenderTarget& target) {
    const sf::Vector2f size = sf::Vector2f(target.getSize()) / kBackgroundScale;

    m_backgroundSprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(size)));
    target.draw(m_backgroundSprite);
}

int main() {
    try {
        SfmlMain example("Shimera - SFML 3 2D example", kWindowWidth, kWindowHeight);

        example.initShimera();
        example.run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
