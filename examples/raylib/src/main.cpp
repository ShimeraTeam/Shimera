#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "effects/PixelisationEffect.hpp"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Raylib - Multi-Pass Post-Processing");

    if (glewInit() != GLEW_OK)
        std::cerr << "[GLEW] initialization failed!" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    Camera camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 25.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << std::endl;
        return -1;
    }

    shimera::IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(screenWidth, screenHeight);

    shimera::PixelisationEffect pixelisationEffect(backend);
    pixelisationEffect.withPixelSize(4.0f)
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
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLACK);
            EndMode3D();
        sceneFramebuffer->unbind();

        // Project cube center to screen space and align the pixel grid on it
        Vector2 cubeScreen = GetWorldToScreen(cubePosition, camera);
        float pixelUVx = pixelisationEffect.m_uPixelSizeX / screenWidth;
        float pixelUVy = pixelisationEffect.m_uPixelSizeY / screenHeight;
        pixelisationEffect.m_uOffset = shimera::Vec2(
            cubeScreen.x / screenWidth - pixelUVx * 0.5f,
            cubeScreen.y / screenHeight - pixelUVy * 0.5f
        );

        BeginDrawing();
            ClearBackground(BLACK);
            pixelisationEffect.render(sceneFramebuffer->getTexture());
        EndDrawing();
    }

    CloseWindow();
    delete sceneFramebuffer;
    delete backend;
    return 0;
}
