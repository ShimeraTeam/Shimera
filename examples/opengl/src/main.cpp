#include <iostream>
#include <array>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"

using namespace shimera;

namespace {

GLFWwindow* initWindow(int width, int height) {
    if (!glfwInit()) {
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW ERROR" << '\n';
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

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
    GLC(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));

    GLC(glGenBuffers(1, &ibo));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));

    GLC(glGenVertexArrays(1, &vao));
    GLC(glBindVertexArray(vao));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLC(glBindVertexArray(0));
}

void loop(GLFWwindow* window, unsigned int shader, unsigned int vao,
          Uniform<Vec4<float>>& colorUniform, IFrameBuffer* sceneFramebuffer, DistortionEffect& distortionEffect) {

    float time = 0.0f;
    float r = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window))
    {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

        GLC(glUseProgram(shader));
        colorUniform = Vec4(r, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment =  0.05f;
        }
        r += increment;

        sceneFramebuffer->unbind();
        distortionEffect.withTime(time);
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));
        distortionEffect.render(sceneFramebuffer->getTexture());

        time += 0.06f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
}

int main()
{
    try {
        GLFWwindow* window = initWindow(640, 480);
        if (!window) {
            return -1;
        }

        unsigned int buffer, ibo, vao;
        setShapes(buffer, ibo, vao);

        const ShaderProgramSource source = parseShader(
            "../../../../res/shader/basic.vert",
            "../../../../res/shader/basic.frag"
        );
        const unsigned int shader = createShader(source.vertex, source.fragment);
        GLC(glUseProgram(shader));

        Uniform colorUniform(shader, "u_Color", Vec4(0.3f, 0.3f, 0.8f, 1.0f));

        IBackend* backend = BackendFactory::create();
        if (!backend) {
            std::cerr << "Failed to create backend!\n";
            GLC(glDeleteProgram(shader));
            GLC(glDeleteVertexArrays(1, &vao));
            GLC(glDeleteBuffers(1, &buffer));
            GLC(glDeleteBuffers(1, &ibo));
            glfwTerminate();
            return -1;
        }

        IFrameBuffer* sceneFramebuffer = backend->createFrameBuffer(640, 480);

        DistortionEffect distortionEffect(backend);
        distortionEffect.withDistortionStrength(0.13f)
                        .withNoiseScale(3.0f)
                        .withTimeScale(0.1f);

        loop(window, shader, vao, colorUniform, sceneFramebuffer, distortionEffect);

        GLC(glDeleteProgram(shader));
        GLC(glDeleteVertexArrays(1, &vao));
        GLC(glDeleteBuffers(1, &buffer));
        GLC(glDeleteBuffers(1, &ibo));
        delete sceneFramebuffer;
        delete backend;
        glfwTerminate();

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}