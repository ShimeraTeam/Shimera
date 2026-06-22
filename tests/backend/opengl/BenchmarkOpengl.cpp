#include <chrono>
#include "BenchmarkOpengl.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

BenchmarkOpengl::BenchmarkOpengl(const std::string &testName, GLFWwindow* window) {
    m_name = testName;
    m_window = window;
}

std::string BenchmarkOpengl::getName() const {
    return m_name;
}

void BenchmarkOpengl::setupScene(BenchmarkReport &report) {

    float positions[] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f};

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2};

    GLC(glGenBuffers(1, &m_buffer));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    GLC(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    GLC(glGenBuffers(1, &m_ibo));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    shimera::ShaderProgramSource source = parseShader(
        "../../../../res/shader/basic.vert",
        "../../../../res/shader/basic.frag");

    m_shader = createShader(source.vertex, source.fragment);
    GLC(glUseProgram(m_shader));

    m_colorUniform = new shimera::Uniform(m_shader, "u_Color", shimera::Vec4(0.3f, 0.3f, 0.8f, 1.0f));

    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glBindVertexArray(m_vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));

    // Unbind for now
    GLC(glBindVertexArray(0));

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
    if (vramBefore >= 0 && vramAfter >= 0)
    {
        usedKb = vramBefore - vramAfter;
        std::cout << "[VRAM BENCH] GPU  : " << glGetString(GL_RENDERER) << "\n";
        std::cout << "[VRAM BENCH] Used : " << usedKb / 1024 << " MB" << " (" << usedKb << " KB)\n";
    }
    report.setVramUsed(usedKb);
}

void BenchmarkOpengl::renderScene(float &time, float &r) {
    float increment = 0.05f;

    m_sceneFramebuffer->bind();
    m_sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

    // Use the basic shader and bind VAO
    GLC(glUseProgram(m_shader));
    *m_colorUniform = shimera::Vec4(r, 0.3f, 0.8f, 1.0f);
    GLC(glBindVertexArray(m_vao));
    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f)
        increment = -0.05f;
    else if (r < 0.0f)
        increment = 0.05f;
    r += increment;

    // Render to screen
    m_sceneFramebuffer->unbind();
    m_distortionEffect->m_uTime = time;
    GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLC(glClear(GL_COLOR_BUFFER_BIT));
    m_distortionEffect->render(m_sceneFramebuffer->getTexture());

    time += 0.06f;

    glfwSwapBuffers(m_window);
}

void BenchmarkOpengl::run() {
    BenchmarkReport report;
    float time = 0.0f;
    float r = 0.0f;

    this->setupScene(report);
    for (int i = 0; i < 100; i++) {
        this->renderScene(time, r);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        this->renderScene(time, r);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    double avgFps  = FRAMES / (totalMs / 1000.0);

    std::cout << "[FPS OPENGL BENCH] Frames    : " << FRAMES   << std::endl;
    std::cout << "[FPS OPENGL BENCH] Avg FPS   : " << avgFps   << std::endl;

    report.setGpu(reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
          .setBackend("OpenGl")
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES);
    report.save("../../../../benchmark-results.json");

    GLC(glDeleteProgram(m_shader));
    GLC(glDeleteVertexArrays(1, &m_vao));
    GLC(glDeleteBuffers(1, &m_buffer));
    GLC(glDeleteBuffers(1, &m_ibo));
    delete m_sceneFramebuffer;
    delete m_backend;
    delete m_colorUniform;
    delete m_distortionEffect;
    glfwTerminate();
    return;
}