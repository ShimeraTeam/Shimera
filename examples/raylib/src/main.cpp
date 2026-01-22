#include "raylib.h"
#include "rlgl.h"
#include <iostream>

#include <GL/glew.h>
#include "shimera.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib example");

    if (glewInit() != GLEW_OK)
        std::cerr << "[GLEW] initialization failed!" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 25.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    PostProcessingQuad postQuad(
        "res/shader/postprocessing/postprocess.vert",
        "res/shader/postprocessing/distortion.frag"
    );

    Uniform uf_time(postQuad.getShader(), "time", 0.0f);
    Uniform uf_noiseScale(postQuad.getShader(), "noiseScale", 3.0f);
    Uniform uf_distortionStrength(postQuad.getShader(), "distortionStrength", 0.13f);
    Uniform uf_timeScale(postQuad.getShader(), "timeScale", 0.1f);

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

        // Render the scene in the texture
        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLACK);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            
            rlDrawRenderBatchActive();
            
            // === OPENGL ===
            postQuad.bindShader();
            uf_time += 0.006f;
            glClear(GL_COLOR_BUFFER_BIT);
            postQuad.render(target.texture.id);
            
            // === END OPENGL ===
            
            // You can keep going to draw with raylib
            DrawFPS(10, 10);
            DrawText("Raylib Post-Processing", 10, 30, 20, LIME);
            
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}