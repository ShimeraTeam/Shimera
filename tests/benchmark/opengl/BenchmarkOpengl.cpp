#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "BenchmarkOpengl.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

static constexpr int FRAMES = 5000;

namespace {

    void makeSphere(int stacks, int slices,
                std::vector<float>& positions,
                std::vector<float>& normals,
                std::vector<unsigned int>& indices) {
        for (int i = 0; i <= stacks; ++i) {
            const float phi = static_cast<float>(i) / static_cast<float>(stacks) * glm::pi<float>();
            for (int j = 0; j <= slices; ++j) {
                const float theta = static_cast<float>(j) / static_cast<float>(slices) * glm::two_pi<float>();
                const float x = std::sin(phi) * std::cos(theta);
                const float y = std::cos(phi);
                const float z = std::sin(phi) * std::sin(theta);
                positions.insert(positions.end(), {x, y, z});
                normals.insert(normals.end(), {x, y, z});
            }
        }
        const int ring = slices + 1;
        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                const unsigned int a = i * ring + j;
                const unsigned int b = a + ring;
                indices.insert(indices.end(), {a, b, a + 1, a + 1, b, b + 1});
            }
        }
    }
}

void BenchmarkOpengl::make2DCube() {
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
}

BenchmarkOpengl::BenchmarkOpengl(const std::string &testName, GLFWwindow* window, shimera::IBackend* backend, 
    shimera::EffectPipeline &&pipeline, GLint vramUsed) : m_backend(backend), m_pipeline(std::move(pipeline)), m_vramUsed(vramUsed) {
    m_name = testName;
    m_window = window;
}

std::string BenchmarkOpengl::getName() const {
    return m_name;
}

void BenchmarkOpengl::setupScene(BenchmarkReport &report) {
    GLint vramBefore = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    if (m_name.find("FresnelEffect") == std::string::npos) {
        make2DCube();
    } else {

        makeSphere(64, 64, m_spPos, m_spNrm, m_spIdx);
        m_sphere = m_backend->createMesh(m_spPos, m_spNrm, m_spIdx);

        m_fresnelEffect = new shimera::FresnelEffect(m_backend);
        m_fresnelEffect->withColor(shimera::Vec3(0.3f, 0.6f, 1.0f))
            .withPower(3.0f)
            .withReflectance(0.04f)
            .withIntensity(1.5f);
        glEnable(GL_DEPTH_TEST);
    }

    // Unbind for now
    GLC(glBindVertexArray(0));

    // check vram

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
    if (m_vramUsed >= 0) {
        usedKb += m_vramUsed;
    }
    report.setVramUsed(usedKb);
}

void BenchmarkOpengl::renderScene(float &time, float &r) {
    float increment = 0.05f;

    if (m_name.find("FresnelEffect") != std::string::npos) {
        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(m_window, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);
        const float aspect = fbh > 0 ? static_cast<float>(fbw) / static_cast<float>(fbh) : 1.0f;
        const shimera::Camera camera = shimera::CameraFactory::perspective(
            shimera::Vec3(10.0f, 10.0f, 10.0f),
            shimera::Vec3(0.0f, 3.0f, 6.0f),
            shimera::Vec3(0.0f, 1.0f, 0.0f),
            45.0f, aspect, 0.1f, 100.0f);

        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_fresnelEffect->setTransform(shimera::Vec3(0.0f, 3.0f, 6.0f));
        m_fresnelEffect->render(*m_sphere, camera);

        glfwSwapBuffers(m_window);
        return;
    }

    if (m_pipeline.size() == 0) {
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));
        GLC(glUseProgram(m_shader));
        *m_colorUniform = shimera::Vec4(r, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(m_vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        glfwSwapBuffers(m_window);
        return;
    }

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
    if (m_pipeline.getEffectsNames().find("DistortionEffect") != std::string::npos) {
        m_pipeline.get<shimera::DistortionEffect>().m_uTime = time;
    }
    GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLC(glClear(GL_COLOR_BUFFER_BIT));
    m_pipeline.render(m_sceneFramebuffer->getTexture());
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
    int totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    int avgFps  = FRAMES / (totalMs / 1000.0);

    report.setGpu(reinterpret_cast<const char*>(glGetString(GL_RENDERER)))
          .setBackend("OpenGl")
          .setEffects(m_pipeline.getEffectsNames())
          .setAvgFps(avgFps)
          .setTotalMs(totalMs)
          .setFrames(FRAMES);

    if (m_name.find("FresnelEffect") == std::string::npos) {
        GLC(glDeleteProgram(m_shader));
        GLC(glDeleteVertexArrays(1, &m_vao));
        GLC(glDeleteBuffers(1, &m_buffer));
        GLC(glDeleteBuffers(1, &m_ibo));
        delete m_colorUniform;
    }
    if (m_pipeline.size() > 0) {
        delete m_sceneFramebuffer;
    }
    if (m_name.find("FresnelEffect") != std::string::npos) {
        report.setEffects("FresnelEffect");
        delete m_fresnelEffect;
        delete m_sphere;
        GLC(glDisable(GL_DEPTH_TEST));
    }
    report.save("../../../../benchmark-results.json");
    return;
}