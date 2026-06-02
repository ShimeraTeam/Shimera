#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include <array>
#include <chrono>
#include <shimera.h>
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

namespace {

GLFWwindow* initWindow(int width, int height) {
    /* Initialize the library */
    if (!glfwInit()) {
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW ERROR" << '\n';
        return nullptr;
    }

    return window;
}

void setShapes(unsigned int& buffer, unsigned int& ibo, unsigned int& vao) {
    const std::array<float, 8> positions = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    const std::array<unsigned int, 6> indices = {
        0, 1, 2,
        1, 3, 2
    };

    GLC(glGenBuffers(1, &buffer));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions.data(), GL_STATIC_DRAW));

    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    GLC(glGenBuffers(1, &ibo));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    shimera::ShaderProgramSource source = parseShader(
        "../../../../res/shader/basic.vert",
        "../../../../res/shader/basic.frag"
        );

    unsigned int shader = createShader(source.vertex, source.fragment);
    GLC(glUseProgram(shader));

    shimera::Uniform colorUniform(shader, "u_Color", shimera::Vec4(0.3f, 0.3f, 0.8f, 1.0f));

    GLC(glGenVertexArrays(1, &vao));
    GLC(glBindVertexArray(vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // Unbind for now
    GLC(glBindVertexArray(0));
}

void render(GLFWwindow* window, unsigned int shader, unsigned int vao,
            shimera::Uniform<shimera::Vec4<float>>& colorUniform, shimera::IFrameBuffer* sceneFramebuffer, shimera::DistortionEffect& distortionEffect, float &time, float &r, float &increment) {
    sceneFramebuffer->bind();
    sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

    // Use the basic shader and bind VAO
    GLC(glUseProgram(shader));
    colorUniform = shimera::Vec4(r, 0.3f, 0.8f, 1.0f);
    GLC(glBindVertexArray(vao));
    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f) {
        increment = -0.05f;
    }
    else if (r < 0.0f) {
        increment = 0.05f;
    }
    r += increment;

    // Render to screen
    sceneFramebuffer->unbind();
    distortionEffect.m_uTime = time;
    GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLC(glClear(GL_COLOR_BUFFER_BIT));
    distortionEffect.render(sceneFramebuffer->getTexture());

    time += 0.06f;

    glfwSwapBuffers(window);
}

}

int main() {
    try {
        GLFWwindow* window = initWindow(640, 480);
        if (!window) {
            return -1;
        }

        unsigned int buffer, ibo, vao;
        setShapes(buffer, ibo, vao);

        const shimera::ShaderProgramSource source = parseShader(
            "../../../../res/shader/basic.vert",
            "../../../../res/shader/basic.frag"
            );

        const unsigned int shader = createShader(source.vertex, source.fragment);
        GLC(glUseProgram(shader));

        shimera::Uniform colorUniform(shader, "u_Color", shimera::Vec4(0.3f, 0.3f, 0.8f, 1.0f));

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

        if (vramBefore >= 0 && vramAfter >= 0) {
            const GLint usedKb = vramBefore - vramAfter;
            std::cout << "[VRAM BENCH] GPU  : " << glGetString(GL_RENDERER) << "\n";
            std::cout << "[VRAM BENCH] Used : " << usedKb / 1024 << " MB" << " (" << usedKb << " KB)\n";
        }

        float time = 0.0f;
        float r = 0.0f;
        float increment = 0.05f;

        // few seconds to be sure the GPU is ready
        for (int i = 0; i < 100; i++) {
            render(window, shader, vao, colorUniform, sceneFramebuffer, distortionEffect, time, r, increment);
        }

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < FRAMES; i++) {
            render(window, shader, vao, colorUniform, sceneFramebuffer, distortionEffect, time, r, increment);
        }

        auto end = std::chrono::high_resolution_clock::now();
        const double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
        const double avgFps  = FRAMES / (totalMs / 1000.0);

        std::cout << "[FPS OPENGL BENCH] Frames    : " << FRAMES   << '\n';
        std::cout << "[FPS OPENGL BENCH] Avg FPS   : " << avgFps   << '\n';

        GLC(glDeleteProgram(shader));
        GLC(glDeleteVertexArrays(1, &vao));
        GLC(glDeleteBuffers(1, &buffer));
        GLC(glDeleteBuffers(1, &ibo));
        delete sceneFramebuffer;
        delete backend;
        glfwTerminate();
        exit(0);

    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

        // Use the basic shader and bind VAO
        GLC(glUseProgram(shader));
        colorUniform = shimera::Vec4(r, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) increment = -0.05f;
        else if (r < 0.0f) increment = 0.05f;
        r += increment;

        // Render to screen
        sceneFramebuffer->unbind();
        distortionEffect.m_uTime = time;
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));
        distortionEffect.render(sceneFramebuffer->getTexture());

        time += 0.06f;

        glfwSwapBuffers(window);
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
          .setFrames(FRAMES)
          .setVramUsed(usedKb);
    report.save("../../../../bench.json");

    GLC(glDeleteProgram(shader));
    GLC(glDeleteVertexArrays(1, &vao));
    GLC(glDeleteBuffers(1, &buffer));
    GLC(glDeleteBuffers(1, &ibo));
    delete sceneFramebuffer;
    delete backend;
    glfwTerminate();
    exit(0);
}