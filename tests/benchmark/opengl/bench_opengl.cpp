#include <GLFW/glfw3.h>
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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR" << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int buffer;
    GLC(glGenBuffers(1, &buffer));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));

    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int ibo;
    GLC(glGenBuffers(1, &ibo));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = parseShader(
        "../../../../res/shader/basic.vert",
        "../../../../res/shader/basic.frag"
        );

    unsigned int shader = createShader(source.vertex, source.fragment);
    GLC(glUseProgram(shader));

    Uniform colorUniform(shader, "u_Color", Vec4(0.3f, 0.3f, 0.8f, 1.0f));

    unsigned int vao;
    GLC(glGenVertexArrays(1, &vao));
    GLC(glBindVertexArray(vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // Unbind for now
    GLC(glBindVertexArray(0));

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

    float time = 0.0f;
    float r = 0.0f;
    float increment = 0.05f;

    // few seconds to be sure the GPU is ready
    for (int i = 0; i < 100; i++) {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

        // Use the basic shader and bind VAO
        GLC(glUseProgram(shader));
        colorUniform = Vec4(r, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) increment = -0.05f;
        else if (r < 0.0f) increment = 0.05f;
        r += increment;

        // Render to screen
        sceneFramebuffer->unbind();
        distortionEffect.time = time;
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));
        distortionEffect.render(sceneFramebuffer->getTexture());

        time += 0.06f;

        glfwSwapBuffers(window);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < FRAMES; i++) {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

        // Use the basic shader and bind VAO
        GLC(glUseProgram(shader));
        colorUniform = Vec4(r, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) increment = -0.05f;
        else if (r < 0.0f) increment = 0.05f;
        r += increment;

        // Render to screen
        sceneFramebuffer->unbind();
        distortionEffect.time = time;
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

    GLC(glDeleteProgram(shader));
    GLC(glDeleteVertexArrays(1, &vao));
    GLC(glDeleteBuffers(1, &buffer));
    GLC(glDeleteBuffers(1, &ibo));
    delete sceneFramebuffer;
    delete backend;
    glfwTerminate();
    exit(0);
}