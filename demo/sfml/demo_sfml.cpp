#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <iostream>
#include <map>
#include <functional>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/BrightnessEffect.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/ColorshiftEffect.hpp"
#include "effects/ContrastEffect.hpp"
#include "effects/GrayscaleEffect.hpp"
#include "effects/SaturationEffect.hpp"
#include "effects/VignetteEffect.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({960, 540}), "shimera demo - sfml");
    window.setActive(true);
    glewInit();

    IBackend *backend = BackendFactory::create();
    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(960, 540);

    DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.2f)
                    .withNoiseScale(4.0f);

    BrightnessEffect brightnessEffect(backend);
    brightnessEffect.withStrength(0.5f);

    ChromaticAberrationEffect chromaticAberrationEffect(backend);
    
    ColorshiftEffect colorshiftEffect(backend);

    ContrastEffect contrastEffect(backend);
    contrastEffect.withContrastFactor(0.5f);

    GrayscaleEffect grayscaleEffect(backend);

    SaturationEffect saturationEffect(backend);
    saturationEffect.withStrength(0.6f);

    VignetteEffect vignetteEffect(backend);
    vignetteEffect.withRadius(0.3f);

    sf::Texture imageTexture;
    if (!imageTexture.loadFromFile("../../../../demo/assets/image.jpg")) {
        return -1;
    }

    sf::Sprite sprite(imageTexture);

    float time = 0.0f;

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (!window.setActive(true))
            break;

        auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(sceneFramebuffer->getNativeRenderTarget());
        sfmlRenderTexture->clear(sf::Color::Black);

        sfmlRenderTexture->draw(sprite);

        sceneFramebuffer->unbind();
        window.setActive(true);

        distortionEffect.time = time;
        distortionEffect.render(sceneFramebuffer->getTexture());
        
        //brightnessEffect.render(sceneFramebuffer->getTexture());

        //chromaticAberrationEffect.render(sceneFramebuffer->getTexture());

        //colorshiftEffect.render(sceneFramebuffer->getTexture());

        //contrastEffect.render(sceneFramebuffer->getTexture());

        //grayscaleEffect.render(sceneFramebuffer->getTexture());

        //saturationEffect.render(sceneFramebuffer->getTexture());

        //vignetteEffect.render(sceneFramebuffer->getTexture());

        time += 0.006f;
        window.display();
    }

    delete sceneFramebuffer;
    delete backend;
    window.close();
    return 0;
}