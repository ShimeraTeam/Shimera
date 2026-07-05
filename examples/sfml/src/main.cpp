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
#include "effects/GaussianBlurEffect.hpp"
#include "effects/HDRBloomEffect.hpp"
#include "EffectPipeline.inl"
#include "effects/VignetteEffect.hpp"

using namespace shimera;


int main()
{
    const sf::VideoMode videoMode({960, 540});
    sf::RenderWindow window(videoMode, "SFML3 - Nice Multi-pass Post-processing");
    window.setActive(true);

    //TODO: Try to embed that in the backend so the user doesn't have to worry about it (or at least make it optional)
    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << '\n';
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    IBackend *backend = BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        return -1;
    }

    // Create framebuffer for the scene
    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(960, 540);

    HDRBloomEffect hdrBloomEffect(backend);
    hdrBloomEffect.withThreshold(0.85f)
                  .withKnee(0.4f)
                  .withIntensity(0.5f)
                  .withBlurSigma(6.0f)
                  .withBlurSamples(18)
                  .withResolution(Vec2(960.0f, 540.0f));

    // Effect Pipeline: Managing fbo passes automatically
    EffectPipeline pipeline(backend, videoMode.size.x, videoMode.size.y);
    pipeline.addEffect<DistortionEffect>()
            .addEffect(std::move(hdrBloomEffect))
            .addEffect<VignetteEffect>(1.0f, 0.4f, 0.3f)
            .build();

    // sf::CircleShape circle(80.f);
    // circle.setFillColor(sf::Color::Magenta);
    // circle.setPosition(sf::Vector2f(210.f - 80.f, 270.f - 80.f));

    // sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    // rectangle.setFillColor(sf::Color::White);
    // rectangle.setPosition(sf::Vector2f(480.f - 80.f, 270.f - 80.f));

    // sf::CircleShape triangle(105.f, 3);
    // triangle.setFillColor(sf::Color::Yellow);
    // triangle.setPosition(sf::Vector2f(750.f - 105.f, 270.f - 80.f));

    sf::Texture texture;
    if (!texture.loadFromFile("../../../../examples/res/assets/image_test.jpg")) {
        std::cerr << "Error loading image" << '\n';
        return -1;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f(0.f, 0.f));
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));

    sf::Clock clock;
    clock.start();
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (!window.setActive(true)) {
            break;
        }

        pipeline.get<DistortionEffect>().m_uTime = clock.getElapsedTime().asSeconds();

        // Render scene to the framebuffer
        auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());

        sfmlRenderTexture->clear(sf::Color::Black);
        sfmlRenderTexture->draw(sprite);
        // sfmlRenderTexture->draw(circle);
        // sfmlRenderTexture->draw(rectangle);
        // sfmlRenderTexture->draw(triangle);
        sceneFramebuffer->unbind(); // Calls display() internally

        // Apply HDR bloom and render to screen
        window.setActive(true);
        glClear(GL_COLOR_BUFFER_BIT);
        pipeline.render(sceneFramebuffer->getTexture());

        window.display();
    }
    clock.stop();

    // Cleanup
    //TODO: Maybe try to auto clean this (do that in the destructor of the respective classes)
    delete sceneFramebuffer;
    delete backend;

    exit(EXIT_SUCCESS);
    return 0;
}