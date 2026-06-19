#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "effects/HDRBloomEffect.hpp"

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

    const Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        return -1;
    }

    shimera::IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(screenWidth, screenHeight);

    shimera::HDRBloomEffect hdrBloomEffect(backend);
    hdrBloomEffect.withThreshold(0.5f)
                  .withIntensity(1.5f)
                  .withBlurSigma(20.0f)
                  .withBlurSamples(60)
                  .withResolution(shimera::Vec2(static_cast<float>(screenWidth), static_cast<float>(screenHeight)));

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

        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(camera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, YELLOW);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLACK);
            EndMode3D();
        sceneFramebuffer->unbind();

        BeginDrawing();
            ClearBackground(BLACK);
            hdrBloomEffect.render(sceneFramebuffer->getTexture());
        EndDrawing();
    }

    CloseWindow();
    delete sceneFramebuffer;
    delete backend;
    return 0;
}
