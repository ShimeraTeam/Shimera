#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>

#include "EffectPipeline.inl"
#include "backend/BackendFactory.hpp"
#include "effects/AtmosphericScatteringEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/GaussianBlurEffect.hpp"
#include "effects/PixelisationEffect.hpp"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Raylib - Multi-Pass Post-Processing");

    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << '\n';
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    Camera camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 25.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    auto spherePosition = Vector3(0.0f, 0.0f, 0.0f);

    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        return -1;
    }

    shimera::IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(screenWidth, screenHeight, true);

    shimera::EffectPipeline effects(backend, 960, 540);
    effects.addEffect<shimera::GaussianBlurEffect>()
        .addEffect<shimera::AtmosphericScatteringEffect>()
        .addEffect<shimera::DistortionEffect>()
        .build();

    auto &blur = effects.get<shimera::GaussianBlurEffect>();
    blur.withSigma(5.0f)
      .withSamples(15)
      .withResolution(shimera::Vec2(960.0f, 540.0f));

    auto &atmo = effects.get<shimera::AtmosphericScatteringEffect>();
    atmo.withPlanet({0,0,0}, 15.0f, 23.6f)
        .withSun({100, 100, 0});

    auto &distortion = effects.get<shimera::DistortionEffect>();
    distortion.withDistortionStrength(0.06f);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (GetMouseWheelMove() != 0)
        {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }

        atmo.m_uCameraPos = shimera::Vec3(camera.position.x, camera.position.y, camera.position.z);
        atmo.m_uCameraTarget = shimera::Vec3(camera.target.x, camera.target.y, camera.target.z);
        atmo.m_uCameraUp = shimera::Vec3(camera.up.x, camera.up.y, camera.up.z);
        atmo.m_fovYDegrees = camera.fovy;

        atmo.setDepthTexture(sceneFramebuffer->getDepthTexture());

        distortion.m_uTime = static_cast<float>(GetTime());

        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(camera);
                // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLACK);
                DrawSphere(spherePosition, 20.0f, {.r=53, .g=88, .b=29, .a=255});
            EndMode3D();
        sceneFramebuffer->unbind();

        BeginDrawing();
            ClearBackground(BLACK);
            effects.render(sceneFramebuffer->getTexture(), &sceneFramebuffer->getDepthTexture());
        EndDrawing();
    }

    CloseWindow();
    delete sceneFramebuffer;
    delete backend;
    return 0;
}
