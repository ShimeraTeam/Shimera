#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

int main() {
    sf::RenderWindow window(sf::VideoMode({960, 540}), "shimera bench - sfml");
    window.setActive(true);
    glewInit();

    // check vram
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);

    shimera::IBackend *backend = shimera::BackendFactory::create();
    shimera::IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(960, 540);

    shimera::DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.2f)
                    .withNoiseScale(4.0f);

    glFinish();

    GLint vramAfter = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);

    if (vramBefore >= 0 && vramAfter >= 0) {
        const GLint usedKb = vramBefore - vramAfter;
        std::cout << "[VRAM BENCH] GPU  : " << glGetString(GL_RENDERER) << "\n";
        std::cout << "[VRAM BENCH] Used : " << usedKb / 1024 << " MB" << " (" << usedKb << " KB)\n";
    }

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Magenta);
    circle.setPosition(sf::Vector2f(210.f - 80.f, 270.f - 80.f));

    sf::RectangleShape rectangle(sf::Vector2f(160.f, 160.f));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(sf::Vector2f(480.f - 80.f, 270.f - 80.f));

    sf::CircleShape triangle(105.f, 3);
    triangle.setFillColor(sf::Color::Yellow);
    triangle.setPosition(sf::Vector2f(750.f - 105.f, 270.f - 80.f));
    float time = 0.0f;

    // few seconds to be sure the GPU is ready
    for (int i = 0; i < 100; i++) {
        auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());
        sfmlRenderTexture->clear(sf::Color::Black);
        sfmlRenderTexture->draw(circle);
        sfmlRenderTexture->draw(rectangle);
        sfmlRenderTexture->draw(triangle);
        sceneFramebuffer->unbind();
        window.setActive(true);
        distortionEffect.m_uTime = time;
        distortionEffect.render(sceneFramebuffer->getTexture());
        time += 0.006f;
        window.display();
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());
        sfmlRenderTexture->clear(sf::Color::Black);
        sfmlRenderTexture->draw(circle);
        sfmlRenderTexture->draw(rectangle);
        sfmlRenderTexture->draw(triangle);
        sceneFramebuffer->unbind();
        window.setActive(true);
        distortionEffect.m_uTime = time;
        distortionEffect.render(sceneFramebuffer->getTexture());
        time += 0.006f;
        window.display();
    }

    auto end = std::chrono::high_resolution_clock::now();
    const double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    const double avgFps  = FRAMES / (totalMs / 1000.0);

    std::cout << "[FPS SFML BENCH] Frames    : " << FRAMES   << '\n';
    std::cout << "[FPS SFML BENCH] Avg FPS   : " << avgFps   << '\n';

    delete sceneFramebuffer;
    delete backend;
    window.close();
    exit(0);
}