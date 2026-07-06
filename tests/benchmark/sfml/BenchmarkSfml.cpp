#include <chrono>
#include "BenchmarkSfml.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

BenchmarkSfml::BenchmarkSfml(const std::string &testName, sf::RenderWindow &window, shimera::IBackend* backend, 
    shimera::EffectPipeline &&pipeline, GLint vramUsed) : m_window(window), m_backend(backend), m_pipeline(std::move(pipeline)), m_vramUsed(vramUsed) {
    m_name = testName;
}

std::string BenchmarkSfml::getName() const {
    return m_name;
}

void BenchmarkSfml::setupScene(BenchmarkReport &report) {

    // check vram
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);

    if (m_pipeline.size() > 0) {
        m_sceneFramebuffer = m_backend->createFrameBuffer(960, 540);
        m_pipeline.build();
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

    m_circle = sf::CircleShape(80.f);
    m_circle.setFillColor(sf::Color::Magenta);
    m_circle.setPosition(sf::Vector2f(210.f - 80.f, 270.f - 80.f));

    m_rectangle = sf::RectangleShape(sf::Vector2f(160.f, 160.f));
    m_rectangle.setFillColor(sf::Color::White);
    m_rectangle.setPosition(sf::Vector2f(480.f - 80.f, 270.f - 80.f));

    m_triangle = sf::CircleShape(105.f, 3);
    m_triangle.setFillColor(sf::Color::Yellow);
    m_triangle.setPosition(sf::Vector2f(750.f - 105.f, 270.f - 80.f));
}

void BenchmarkSfml::renderScene(float &time) {
    if (m_pipeline.size() == 0) {
        m_window.clear(sf::Color::Black);
        m_window.draw(m_circle);
        m_window.draw(m_rectangle);
        m_window.draw(m_triangle);
        m_window.display();
        return;
    }
    auto *sfmlRenderTexture = static_cast<sf::RenderTexture*>(m_sceneFramebuffer->getNativeRenderTarget());
    sfmlRenderTexture->clear(sf::Color::Black);
    sfmlRenderTexture->draw(m_circle);
    sfmlRenderTexture->draw(m_rectangle);
    sfmlRenderTexture->draw(m_triangle);
    m_sceneFramebuffer->unbind();
    m_window.setActive(true);
    if (m_pipeline.getEffectsNames().find("DistortionEffect") != std::string::npos) {
        m_pipeline.get<shimera::DistortionEffect>().m_uTime = time;
    }
    m_pipeline.render(m_sceneFramebuffer->getTexture());
    time += 0.006f;
    m_window.display();
}

void BenchmarkSfml::run() {
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
          .setBackend("Sfml")
          .setEffects(m_pipeline.getEffectsNames())
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES);
    report.save("../../../../benchmark-results.json");

    if (m_pipeline.size() > 0) {
        delete m_sceneFramebuffer;
    }
    return;
}