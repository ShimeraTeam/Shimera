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


int main()
{
    const sf::VideoMode videoMode({800, 400});
    sf::RenderWindow window(videoMode, "SFML3 - Multi-Pass Post-Processing");
    window.setActive(true);

    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    IBackend* backend = BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << std::endl;
        return -1;
    }

    // Create framebuffers for ping-pong rendering
    IFrameBuffer* sceneFramebuffer = backend->createFrameBuffer(800, 400);
    IFrameBuffer* tempFramebuffer = backend->createFrameBuffer(800, 400);  // Intermediate pass

    IPostProccessor* distortionEffect = backend->createPostProcessor(
        "../../../../res/shader/postprocessing/postprocess.vert",
        "../../../../res/shader/postprocessing/distortion.frag"
    );

    IPostProccessor* grayscaleEffect = backend->createPostProcessor(
        "../../../../res/shader/postprocessing/postprocess.vert",
        "../../../../res/shader/postprocessing/grayscale.frag"
    );

    distortionEffect->setUniform("noiseScale", 3.0f);
    distortionEffect->setUniform("distortionStrength", 0.13f);
    distortionEffect->setUniform("timeScale", 0.1f);

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(175.f - 80.f, 200.f - 80.f));

    sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(sf::Vector2f(400.f - 80.f, 200.f - 80.f));

    sf::CircleShape triangle(105.f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(625.f - 105.f, 200.f - 80.f));

    float time = 0.0f;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Render scene to the first framebuffer ("scene"Framebuffer)
        auto* sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());
        
        sfmlRenderTexture->clear(sf::Color::Black);
        sfmlRenderTexture->draw(circle);
        sfmlRenderTexture->draw(rectangle);
        sfmlRenderTexture->draw(triangle);
        sceneFramebuffer->unbind(); // Calls display() internally
        
        // Ensure window context is active for post-processing
        window.setActive(true);
        
        // Multi-pass rendering chain:
        // 1. sceneFramebuffer (original) -> distortion -> tempFramebuffer
        // 2. tempFramebuffer -> grayscale -> screen
        
        // Pass 1: Apply distortion to intermediate Framebuffer
        tempFramebuffer->bind(); // Activate tempFramebuffer's OpenGL context
        glClear(GL_COLOR_BUFFER_BIT);
        distortionEffect->setUniform("time", time);
        distortionEffect->render(sceneFramebuffer->getTexture()); // "Set" the distortion effect to the scene fb
        tempFramebuffer->unbind();
        
        // Pass 2: Apply grayscale to screen
        window.setActive(true); // Switch back to window (SFML) context
        glClear(GL_COLOR_BUFFER_BIT);
        grayscaleEffect->render(tempFramebuffer->getTexture());
        
        time += 0.006f;
        
        window.display();
    }

    // Cleanup
    //TODO: Maybe try to auto clean this (do that in the destructor of the respective classes)
    delete grayscaleEffect;
    delete distortionEffect;
    delete tempFramebuffer;
    delete sceneFramebuffer;
    delete backend;

    return 0;
}
