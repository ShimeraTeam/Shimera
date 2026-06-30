#include "raylib.h"
#include <iostream>
#include <GL/glew.h>

#include <shimera.h>

#include "EffectPipeline.inl"
#include "backend/BackendFactory.hpp"
#include "backend/raylib/converts/RaylibCamera.hpp"
#include "backend/raylib/RaylibMesh.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/materials/FresnelEffect.hpp"
#include "scene/Camera.hpp"

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

    shimera::Camera shCam = shimera::RaylibCamera::toShimera(camera);

    // Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    auto spherePosition = shimera::Vec3(0.0f, 0.0f, 0.0f);

    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        return -1;
    }

    Mesh sphereMesh = GenMeshSphere(1.5f, 48, 48);
    shimera::RaylibMesh shMesh(sphereMesh);

    shimera::FresnelEffect fresnelMat(backend);
    fresnelMat.withColor(shimera::Vec3(0.3f, 0.7f, 1.0f))
           .withPower(3.0f)
           .withReflectance(0.04f)
           .withIntensity(1.5f);

    fresnelMat.setTransform(spherePosition);

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

        shCam = shimera::RaylibCamera::toShimera(camera);

        BeginDrawing();
            glClear(GL_DEPTH_BUFFER_BIT);
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawCube({5, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires({5, 0, 0}, 2.0f, 2.0f, 2.0f, WHITE);
            EndMode3D();
            glEnable(GL_DEPTH_TEST);
            fresnelMat.render(shMesh, shCam);
            glDisable(GL_DEPTH_TEST);
        EndDrawing();
    }

    UnloadMesh(sphereMesh);
    delete backend;
    CloseWindow();
    return 0;
}
