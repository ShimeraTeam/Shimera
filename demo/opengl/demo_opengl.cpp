#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/BrightnessEffect.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/ColorshiftEffect.hpp"
#include "effects/ContrastEffect.hpp"
#include "effects/GrayscaleEffect.hpp"
#include "effects/SaturationEffect.hpp"
#include "effects/VignetteEffect.hpp"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "shimera demo - opengl", NULL, NULL);
    if (!window)
    {
        return -1;
    }

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

    GLC(glBindVertexArray(0));

    IBackend *backend = BackendFactory::create();
    if (!backend) {
        return -1;
    }

    IFrameBuffer *sceneFramebuffer = backend->createFrameBuffer(640, 480);

    DistortionEffect distortionEffect(backend);
    distortionEffect.withDistortionStrength(0.13f)
                    .withNoiseScale(3.0f)
                    .withTimeScale(0.1f);

    BrightnessEffect brightnessEffect(backend);
    brightnessEffect.withStrength(0.5f);

    ChromaticAberrationEffect chromaticAberrationEffect(backend);

    ColorshiftEffect colorshiftEffect(backend);

    ContrastEffect contrastEffect(backend);
    contrastEffect.withContrastFactor(0.5f);

    GrayscaleEffect grayscaleEffect(backend);

    SaturationEffect saturationEffect(backend);
    saturationEffect.withStrength(0.6f);

    VignetteEffect vignetteEffect(backend);
    vignetteEffect.withRadius(0.3f);

    float time = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        sceneFramebuffer->bind();
        sceneFramebuffer->clear(shimera::Color(0.0f, 0.0f, 0.0f, 1.0f));

        GLC(glUseProgram(shader));
        colorUniform = Vec4(0.3f, 0.3f, 0.8f, 1.0f);
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        sceneFramebuffer->unbind();
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));

        distortionEffect.time = time;
        distortionEffect.render(sceneFramebuffer->getTexture());

        //brightnessEffect.render(sceneFramebuffer->getTexture());

        //chromaticAberrationEffect.render(sceneFramebuffer->getTexture());

        //colorshiftEffect.render(sceneFramebuffer->getTexture());

        //contrastEffect.render(sceneFramebuffer->getTexture());

        //grayscaleEffect.render(sceneFramebuffer->getTexture());

        //saturationEffect.render(sceneFramebuffer->getTexture());

        //vignetteEffect.render(sceneFramebuffer->getTexture());

        time += 0.06f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GLC(glDeleteProgram(shader));
    GLC(glDeleteVertexArrays(1, &vao));
    GLC(glDeleteBuffers(1, &buffer));
    GLC(glDeleteBuffers(1, &ibo));
    delete sceneFramebuffer;
    delete backend;
    glfwTerminate();
    return 0;
}