#include <gtest/gtest.h>
#include "shimera.h"

#ifdef SHIMERA_BACKEND_OPENGL
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#elif defined(SHIMERA_BACKEND_SFML)
    #include <SFML/Window.hpp>
#elif defined(SHIMERA_BACKEND_RAYLIB)
    #include <raylib.h>
#endif

class ContextTest : public ::testing::Test {
protected:

#ifdef SHIMERA_BACKEND_OPENGL
    GLFWwindow* m_window = nullptr;
#elif defined(SHIMERA_BACKEND_SFML)
    sf::Window* m_window = nullptr;
#endif

    void SetUp() override {
#ifdef SHIMERA_BACKEND_OPENGL
        glfwInit();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        m_window = glfwCreateWindow(1, 1, "Opengl window", nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
#elif defined(SHIMERA_BACKEND_SFML)
        m_window = new sf::Window(sf::VideoMode({640, 480}), "SFML window", sf::Style::None);
        if (!m_window->setActive(true)) {
            return;
        }
#elif defined(SHIMERA_BACKEND_RAYLIB)
        InitWindow(640, 480, "Raylib window");
#endif
        glewInit();
    }

    void TearDown() override {
#ifdef SHIMERA_BACKEND_OPENGL
        glfwDestroyWindow(m_window);
        glfwTerminate();
#elif defined(SHIMERA_BACKEND_SFML)
        m_window->close();
        delete m_window;
#elif defined(SHIMERA_BACKEND_RAYLIB)
        CloseWindow();
#endif
    }
};