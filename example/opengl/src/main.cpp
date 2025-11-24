#include <iostream>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "Framebuffer.h"
#include "glUtils.h"
#include "PostProcessingQuad.h"

int main(void)
{
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

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

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
        "./res/shader/basic.vert",
        "./res/shader/basic.frag"
        );

    unsigned int shader = createShader(source.vertex, source.fragment);
    GLC(glUseProgram(shader));

    GLC(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != 1);
    GLC(glUniform4f(location, 0.3, 0.3, 0.8, 1.0));

    unsigned int vao;
    GLC(glGenVertexArrays(1, &vao));
    GLC(glBindVertexArray(vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLC(glEnableVertexAttribArray(0));
    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // Unbind for now
    GLC(glBindVertexArray(0));

    Framebuffer framebuffer(640, 480);
    PostProcessingQuad postQuad(
        "./res/shader/postprocessing/postprocess.vert",
        "./res/shader/postprocessing/colorshift.frag"
    );

    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /// Render into our framebuffer
        framebuffer.bind();
        GLC(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));

        // Use the basic shader and bind VAO
        GLC(glUseProgram(shader));
        GLC(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLC(glBindVertexArray(vao));
        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) increment = -0.05f;
        else if (r < 0.0f) increment = 0.05f;
        r += increment;

        // Render to screen
        framebuffer.unbind();
        GLC(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLC(glClear(GL_COLOR_BUFFER_BIT));
        postQuad.render(framebuffer.getTexture());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GLC(glDeleteProgram(shader));
    GLC(glDeleteVertexArrays(1, &vao));
    GLC(glDeleteBuffers(1, &buffer));
    GLC(glDeleteBuffers(1, &ibo));
    glfwTerminate();
    return 0;
}
