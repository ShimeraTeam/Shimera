#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>
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
    const int screenWidth  = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "shimera demo - raylib");

    if (glewInit() != GLEW_OK)
        std::cerr << "[GLEW] initialization failed!" << std::endl;

    Texture2D imageTexture = LoadTexture("../../../../demo/assets/image.jpg");
    if (imageTexture.id == 0) {
        return -1;
    }

    IBackend *backend = BackendFactory::create();
    if (!backend) {
        return -1;
    }

    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(screenWidth, screenHeight);

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

    SetTargetFPS(60);
    float time = 0.0f;

    while (!WindowShouldClose())
    {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            DrawTexture(imageTexture, 0, 0, WHITE);
        sceneFramebuffer->unbind();

        BeginDrawing();
            ClearBackground(BLACK);

            //distortionEffect.time = time;
            //distortionEffect.render(sceneFramebuffer->getTexture());

            //brightnessEffect.render(sceneFramebuffer->getTexture());

            //chromaticAberrationEffect.render(sceneFramebuffer->getTexture());

            //colorshiftEffect.render(sceneFramebuffer->getTexture());

            //contrastEffect.render(sceneFramebuffer->getTexture());

            //grayscaleEffect.render(sceneFramebuffer->getTexture());

            //saturationEffect.render(sceneFramebuffer->getTexture());

            vignetteEffect.render(sceneFramebuffer->getTexture());

            time += 0.006f;
        EndDrawing();
    }

    UnloadTexture(imageTexture);
    delete sceneFramebuffer;
    delete backend;
    CloseWindow();
    return 0;
}