#include "raylib.h"
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "../BenchmarkReport.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

int main() {
    BenchmarkReport report;
    InitWindow(960, 540, "shimera bench - raylib");
    glewInit();
    glFinish();

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

    GLint usedKb;
    if (vramBefore >= 0 && vramAfter >= 0) {
        usedKb = vramBefore - vramAfter;
        std::cout << "[VRAM BENCH] GPU  : " << glGetString(GL_RENDERER) << "\n";
        std::cout << "[VRAM BENCH] Used : " << usedKb / 1024 << " MB" << " (" << usedKb << " KB)\n";
    }

    Camera camera = { 0 };
    camera.position   = { 10.0f, 10.0f, 10.0f };
    camera.target     = { 0.0f, 0.0f, 0.0f };
    camera.up         = { 0.0f, 1.0f, 0.0f };
    camera.fovy       = 25.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

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
            distortionEffect.m_uTime = time;
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
            distortionEffect.m_uTime = time;
            distortionEffect.render(sceneFramebuffer->getTexture());
            time += 0.006f;
        EndDrawing();
    }

    auto end = std::chrono::high_resolution_clock::now();
    const double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    const double avgFps  = FRAMES / (totalMs / 1000.0);

    std::cout << "[FPS RAYLIB BENCH] Frames    : " << FRAMES   << '\n';
    std::cout << "[FPS RAYLIB BENCH] Avg FPS   : " << avgFps   << '\n';

    report.setGpu(reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
          .setBackend("Raylib")
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES)
          .setVramUsed(usedKb);
    report.save("../../../../bench.json");

    delete sceneFramebuffer;
    delete backend;
    CloseWindow();
    exit(0);
}