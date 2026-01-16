#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <shimera.h>


int main()
{
    sf::VideoMode videoMode({800, 400});
    sf::RenderWindow window(videoMode, "SFML3 - Post-Processing with OpenGL");
    window.setActive(true);

    if (glewInit() != GLEW_OK)
        std::cerr << "[GLEW] initialization failed!" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    sf::RenderTexture renderTexture(videoMode.size);

    Framebuffer framebuffer(videoMode.size.x, videoMode.size.y);
    // May need to change the path manually for now
    PostProcessingQuad postQuad(
    "res/shader/postprocessing/postprocess.vert",
    "res/shader/postprocessing/distortion.frag"
    );

    Uniform uf_time(postQuad.getShader(), "time", 0.0f);
    Uniform uf_noiseScale(postQuad.getShader(), "noiseScale", 3.0f);
    Uniform uf_distortionStrength(postQuad.getShader(), "distortionStrength", 0.13f);
    Uniform uf_timeScale(postQuad.getShader(), "timeScale", 0.1f);

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(175.f - 80.f, 200.f - 80.f));

    sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(sf::Vector2f(400.f - 80.f, 200.f - 80.f));

    sf::CircleShape triangle(105.f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(625.f - 105.f, 200.f - 80.f));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // render what SFML rendered to our --opengl framebuffer-- nope juste the renderTexture for now...
        // (change our context to the framebuffer)
        // framebuffer.bind();

        renderTexture.clear(sf::Color::Black);
        renderTexture.draw(circle);
        renderTexture.draw(rectangle);
        renderTexture.draw(triangle);
        renderTexture.display(); // push the SFML rendered content to the framebuffer

        // apply post-processing effect
        framebuffer.unbind();
        postQuad.bindShader();
        uf_time += 0.006f;
        glClear(GL_COLOR_BUFFER_BIT);
        // TODO: use framebuffer.getTexture() instead of renderTexture.getTexture().getNativeHandle()
        postQuad.render(renderTexture.getTexture().getNativeHandle());

        // reset OpenGL states for SFML (else it won't display correctly)
        // window.resetGLStates();
        window.display(); // display the post-processed result (so display the current OpenGL buffer)
    }

    return 0;
}