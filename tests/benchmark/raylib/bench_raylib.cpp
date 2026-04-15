#include "raylib.h"
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
    InitWindow(960, 540, "shimera bench - raylib");
    glewInit();

    // check vram
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);

    IBackend *backend = BackendFactory::create();
    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(960, 540);
    DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.2f)
                    .withNoiseScale(4.0f);

    glFinish();

    GLint vramAfter = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);

    if (vramBefore >= 0 && vramAfter >= 0) {
        GLint usedKb = vramBefore - vramAfter;
        std::cout << "[VRAM BENCH] Renderer     : " << glGetString(GL_RENDERER) << "\n";
        std::cout << "[VRAM BENCH] Before alloc : " << vramBefore / 1024 << " MB\n";
        std::cout << "[VRAM BENCH] After alloc  : " << vramAfter / 1024 << " MB\n";
        std::cout << "[VRAM BENCH] Used         : " << usedKb / 1024 << " MB" << " (" << usedKb << " KB)\n";
    }

    Camera camera = { 0 };
    camera.position   = { 10.0f, 10.0f, 10.0f };
    camera.target     = { 0.0f, 0.0f, 0.0f };
    camera.up         = { 0.0f, 1.0f, 0.0f };
    camera.fovy       = 25.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    float time = 0.0f;

    // few seconds to be sure the GPU is ready
    for (int i = 0; i < 100; i++) {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(camera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            EndMode3D();
        sceneFramebuffer->unbind();
        BeginDrawing();
            distortionEffect.time = time;
            distortionEffect.render(sceneFramebuffer->getTexture());
            time += 0.006f;
        EndDrawing();
    }

    // check fps
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(camera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            EndMode3D();
        sceneFramebuffer->unbind();
        BeginDrawing();
            distortionEffect.time = time;
            distortionEffect.render(sceneFramebuffer->getTexture());
            time += 0.006f;
        EndDrawing();
    }

    auto end = std::chrono::high_resolution_clock::now();
    double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    double avgFps  = FRAMES / (totalMs / 1000.0);

    std::cout << "[FPS RAYLIB BENCH] Frames    : " << FRAMES   << std::endl;
    std::cout << "[FPS RAYLIB BENCH] Avg FPS   : " << avgFps   << std::endl;

    delete sceneFramebuffer;
    delete backend;
    CloseWindow();
    exit(0);
}