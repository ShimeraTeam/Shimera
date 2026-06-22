#include <chrono>
#include "BenchmarkRaylib.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

BenchmarkRaylib::BenchmarkRaylib(const std::string &testName) {
    m_name = testName;
}

std::string BenchmarkRaylib::getName() const {
    return m_name;
}

void BenchmarkRaylib::setupScene(BenchmarkReport &report) {

    // check vram
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);

    m_backend = shimera::BackendFactory::create();
    m_sceneFramebuffer = m_backend->createFrameBuffer(960, 540);
    m_distortionEffect = new shimera::DistortionEffect(m_backend);
    m_distortionEffect->withDistortionStrength(0.2f)
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
    report.setVramUsed(usedKb);

    m_camera = { 0 };
    m_camera.position   = { 10.0f, 10.0f, 10.0f };
    m_camera.target     = { 0.0f, 0.0f, 0.0f };
    m_camera.up         = { 0.0f, 1.0f, 0.0f };
    m_camera.fovy       = 25.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;

    m_cubePosition = { 0.0f, 0.0f, 0.0f };
}

void BenchmarkRaylib::renderScene(float &time) {
    m_sceneFramebuffer->bind();
        m_sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(m_camera);
                DrawCube(m_cubePosition, 2.0f, 2.0f, 2.0f, RED);
            EndMode3D();
        m_sceneFramebuffer->unbind();
        BeginDrawing();
            m_distortionEffect->m_uTime = time;
            m_distortionEffect->render(m_sceneFramebuffer->getTexture());
            time += 0.006f;
        EndDrawing();
}

void BenchmarkRaylib::run() {
    BenchmarkReport report;
    float time = 0.0f;

    this->setupScene(report);
    for (int i = 0; i < 100; i++) {
        this->renderScene(time);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        this->renderScene(time);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    double avgFps  = FRAMES / (totalMs / 1000.0);

    std::cout << "[FPS RAYLIB BENCH] Frames    : " << FRAMES   << std::endl;
    std::cout << "[FPS RAYLIB BENCH] Avg FPS   : " << avgFps   << std::endl;

    report.setGpu(reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
          .setBackend("Raylib")
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES);
    report.save("../../../../benchmark-results.json");

    delete m_sceneFramebuffer;
    delete m_backend;
    delete m_distortionEffect;
    CloseWindow();
    return;
}