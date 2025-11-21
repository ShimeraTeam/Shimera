#include <iostream>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLC(x) cglClearError();\
    x;\
    ASSERT(cglLogCall(#x, __FILE__, __LINE__));

static void cglClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool cglLogCall(const char *function, const char *file, int line)
{
    while (const GLenum error = glGetError())
    {
        std::cerr << "[OpenGL ERROR] (" << error << "): " << function << " -> " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string vertex;
    std::string fragment;
};

static std::string readFile(const std::string &filePath)
{
    std::ifstream stream(filePath);
    if (!stream.is_open()) {
        std::cerr << "ERROR: can't open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

static ShaderProgramSource parseShader(const std::string &vertexFilePath, const std::string &fragmentFilePath)
{
    ShaderProgramSource source;
    source.vertex = readFile(vertexFilePath);
    source.fragment = readFile(fragmentFilePath);
    return source;
}

unsigned int compileShader(unsigned int type, const std::string &source)
{
    GLC(unsigned int id = glCreateShader(type);)
    const char *str = source.c_str();
    GLC(glShaderSource(id, 1, &str, nullptr));
    GLC(glCompileShader(id));

    int result;
    GLC(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int lenght;
        GLC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        char *message = (char *) alloca(lenght * sizeof(char)); // -> char message[lenght]
        GLC(glGetShaderInfoLog(id, lenght, &lenght, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLC(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string &vertexSource, const std::string &fragmentSource)
{
    GLC(unsigned int program = glCreateProgram());
    GLC(unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource));
    GLC(unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource));

    GLC(glAttachShader(program, vertex));
    GLC(glAttachShader(program, fragment));
    GLC(glLinkProgram(program));
    GLC(glValidateProgram(program));
    GLC(glDeleteShader(vertex));
    GLC(glDeleteShader(fragment));

    return program;
}

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

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLC(glClear(GL_COLOR_BUFFER_BIT));

        GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLC(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}