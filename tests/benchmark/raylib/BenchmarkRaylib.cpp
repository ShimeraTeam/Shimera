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

BenchmarkRaylib::BenchmarkRaylib(const std::string &testName, shimera::IBackend* backend, 
    shimera::EffectPipeline &&pipeline, GLint vramUsed) : m_backend(backend), m_pipeline(std::move(pipeline)), m_vramUsed(vramUsed) {
    m_name = testName;
}

std::string BenchmarkRaylib::getName() const {
    return m_name;
}

void BenchmarkRaylib::setupScene(BenchmarkReport &report) {

    // check vram
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);

    if (m_pipeline.size() > 0) {
        m_sceneFramebuffer = m_backend->createFrameBuffer(960, 540);
        m_pipeline.build();
    }
    if (m_name.find("AtmosphericScatteringEffect") != std::string::npos) {
        m_sceneFramebuffer = m_backend->createFrameBuffer(960, 540, true);
        m_atmosphericScatteringEffect = new shimera::AtmosphericScatteringEffect(m_backend);
        m_atmosphericScatteringEffect->withPlanet({0, 0, 0}, 1.0f, 1.6f)
            .withSun({1.0f, 1.0f, 0.0f})
            .withQuality(16, 16);
    }
    if (m_name.find("FresnelEffect") != std::string::npos) {
        m_model = LoadModelFromMesh(GenMeshSphere(1.5f, 48, 48));
        m_sphereMesh = new shimera::RaylibMesh(m_model);

        m_fresnelEffect = new shimera::FresnelEffect(m_backend);
        m_fresnelEffect->withColor(shimera::Vec3(0.3f, 0.7f, 1.0f))
            .withPower(3.0f)
            .withReflectance(0.04f)
            .withIntensity(1.5f);

        m_fresnelEffect->setTransform(shimera::Vec3(0.0f, 0.0f, 0.0f));
    }
    glFinish();

    GLint vramAfter = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);

    GLint usedKb;
    if (vramBefore >= 0 && vramAfter >= 0) {
        usedKb = vramBefore - vramAfter;
    }
    if (m_vramUsed > 0) {
        usedKb += m_vramUsed;
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
    if (m_name.find("FresnelEffect") != std::string::npos) {
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(m_camera);
                m_fresnelEffect->render(*m_sphereMesh, shimera::RaylibCamera::toShimera(m_camera));
            EndMode3D();
        EndDrawing();
        return;
    }
    if (m_name.find("AtmosphericScatteringEffect") != std::string::npos) {
        m_atmosphericScatteringEffect->m_uCameraPos = shimera::Vec3(m_camera.position.x, m_camera.position.y, m_camera.position.z);
        m_atmosphericScatteringEffect->m_uCameraTarget = shimera::Vec3(m_camera.target.x, m_camera.target.y, m_camera.target.z);
        m_atmosphericScatteringEffect->m_uCameraUp = shimera::Vec3(m_camera.up.x, m_camera.up.y, m_camera.up.z);
        m_atmosphericScatteringEffect->m_fovYDegrees = m_camera.fovy;
        m_atmosphericScatteringEffect->m_aspect = (float)960 / 540;
        m_atmosphericScatteringEffect->setDepthTexture(m_sceneFramebuffer->getDepthTexture());
        m_sceneFramebuffer->bind();
        m_sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(m_camera);
                DrawSphere({0.0f, 0.0f, 0.0f}, 1.0f, {.r = 53, .g = 88, .b = 29, .a = 255});
            EndMode3D();
        m_sceneFramebuffer->unbind();
        BeginDrawing();
            ClearBackground(BLACK);
            m_atmosphericScatteringEffect->render(m_sceneFramebuffer->getTexture());
        EndDrawing();
        return;
    }
    if (m_pipeline.size() == 0) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(m_camera);
                DrawCube(m_cubePosition, 2.0f, 2.0f, 2.0f, RED);
            EndMode3D();
        EndDrawing();
        return;
    }
    m_sceneFramebuffer->bind();
        m_sceneFramebuffer->clear(shimera::Color{0, 0, 0, 1});
            BeginMode3D(m_camera);
                DrawCube(m_cubePosition, 2.0f, 2.0f, 2.0f, RED);
            EndMode3D();
        m_sceneFramebuffer->unbind();
        BeginDrawing();
            if (m_pipeline.getEffectsNames().find("DistortionEffect") != std::string::npos) {
                m_pipeline.get<shimera::DistortionEffect>().m_uTime = time;
            }
            m_pipeline.render(m_sceneFramebuffer->getTexture());
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
    int totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    int avgFps  = FRAMES / (totalMs / 1000.0);

    report.setGpu(reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
          .setBackend("Raylib")
          .setEffects(m_pipeline.getEffectsNames())
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES);

    if (m_pipeline.size() > 0) {
        delete m_sceneFramebuffer;
    }
    if (m_name.find("FresnelEffect") != std::string::npos) {
        report.setEffects("FresnelEffect");
        delete m_fresnelEffect;
        delete m_sphereMesh;
        UnloadModel(m_model);
    }
    if (m_name.find("AtmosphericScatteringEffect") != std::string::npos) {
        report.setEffects("AtmosphericScatteringEffect");
        delete m_atmosphericScatteringEffect;
    }
    report.save("../../../../benchmark-results.json");
    return;
}