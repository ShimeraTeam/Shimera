#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

// Here the shimera header is outdated and should be used to include all necessary shimera components
//TODO: Update `shimera.h` to include all necessary headers
#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "backend/sfml/SFMLFramebuffer.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ColorshiftEffect.hpp"
#include "effects/GrayscaleEffect.hpp"
#include "effects/BrightnessEffect.hpp"
#include "effects/ContrastEffect.hpp"


int main()
{
    const sf::VideoMode videoMode({960, 540});
    sf::RenderWindow window(videoMode, "SFML3 - Multi-Pass Post-Processing");
    window.setActive(true);

    //TODO: Try to embed that in the backend so the user doesn't have to worry about it (or at least make it optional)
    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    IBackend *backend = BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << std::endl;
        return -1;
    }

    // Create framebuffers for ping-pong rendering
    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(960, 540);
    IFrameBuffer *tempFramebuffer = backend->createFrameBuffer(960, 540);  // Intermediate pass

    DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.2f)
                    .withNoiseScale(4.0f);
    GrayscaleEffect grayscaleEffect(backend);

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(210.f - 80.f, 270.f - 80.f));

    sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(sf::Vector2f(480.f - 80.f, 270.f - 80.f));

    sf::CircleShape triangle(105.f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(750.f - 105.f, 270.f - 80.f));

    // To draw a picture, uncomment the line below
    // sf::Texture texture;
    // if (!texture.loadFromFile("../../../../examples/res/test.jpg")) {
    //     std::cerr << "Error loading image" << std::endl;
    // }
    // sf::Sprite sprite(texture);
    // sprite.setPosition(sf::Vector2f(0.f, 0.f));
    // sprite.setScale(sf::Vector2f(0.5f, 0.5f)); // 50% de la taille originale (1920x1080 -> 960x540)

    float time = 0.0f;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Render scene to the first framebuffer ("scene"Framebuffer)
        auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());

        sfmlRenderTexture->clear(sf::Color::Black);
        // sfmlRenderTexture->draw(sprite);
        sfmlRenderTexture->draw(circle);
        sfmlRenderTexture->draw(rectangle);
        sfmlRenderTexture->draw(triangle);
        sceneFramebuffer->unbind(); // Calls display() internally

        // Ensure window context is active for post-processing
        window.setActive(true);

        // Multi-pass rendering chain:
        // 1. sceneFramebuffer (original) -> distortion -> tempFramebuffer
        // 2. tempFramebuffer -> grayscale (saturation=0.0) -> screen
        // Update the necessary uniforms for the distortion effect
        distortionEffect.time = time;
        // Pass 1: Apply distortion -> tempFramebuffer
        tempFramebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        distortionEffect.render(sceneFramebuffer->getTexture());
        tempFramebuffer->unbind();

        // Pass 2: Apply grayscale (saturation = 0.0) -> screen
        window.setActive(true);
        glClear(GL_COLOR_BUFFER_BIT);
        grayscaleEffect.render(tempFramebuffer->getTexture());

        time += 0.006f;

        window.display();
    }

    // Cleanup
    //TODO: Maybe try to auto clean this (do that in the destructor of the respective classes)
    delete tempFramebuffer;
    delete sceneFramebuffer;
    delete backend;

    return 0;
}
