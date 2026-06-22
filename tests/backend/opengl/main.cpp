#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>
#include "../../memory/MemoryTest.hpp"
#include "../../TestRunner.hpp"
#include "BenchmarkOpengl.hpp"

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "OpenGl shimera tests", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR" << std::endl;

    TestRunner runner;
    runner.add(new MemoryTest("Memory Tests"));
    runner.add(new BenchmarkOpengl("Benchmark Opengl", window));
    runner.runAll();

    return 0;
}