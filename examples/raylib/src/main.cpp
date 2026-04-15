#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 400;

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

    IBackend *backend = BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << std::endl;
        return -1;
    }

    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(800, 400);

    DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.2f)
                    .withNoiseScale(4.0f);

    SetTargetFPS(60);
    float time = 0.0f;

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

        BeginDrawing();
            ClearBackground(BLACK);
            distortionEffect.time = time;
            distortionEffect.render(sceneFramebuffer->getTexture());
            time += 0.006f;
            
        EndDrawing();
    }

    CloseWindow();
    delete sceneFramebuffer;
    delete backend;
    return 0;
}