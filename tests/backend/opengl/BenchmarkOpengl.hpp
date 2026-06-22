#ifndef SHIMERA_BENCHMARKOPENGL_HPP
#define SHIMERA_BENCHMARKOPENGL_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "../../BenchmarkReport.hpp"

class BenchmarkOpengl : public ITest {
public:
    BenchmarkOpengl() = default;
    BenchmarkOpengl(const std::string &testName, GLFWwindow* window);
    ~BenchmarkOpengl() override = default;

    std::string getName() const override;
    void run() override;
    void setupScene(BenchmarkReport &report);
    void renderScene(float &time, float &r);

    private:
        std::string m_name;
        GLFWwindow* m_window;
        unsigned int m_vao;
        unsigned int m_shader;
        unsigned int m_ibo;
        unsigned int m_buffer;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::DistortionEffect* m_distortionEffect;
        shimera::Uniform<shimera::Vec4<float>>* m_colorUniform;
};

#endif //SHIMERA_BENCHMARKOPENGL_HPP