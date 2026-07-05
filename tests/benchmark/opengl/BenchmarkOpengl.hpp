#ifndef SHIMERA_BENCHMARKOPENGL_HPP
#define SHIMERA_BENCHMARKOPENGL_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "../../BenchmarkReport.hpp"
#include "EffectPipeline.inl"

class BenchmarkOpengl : public ITest {
    public:
        //BenchmarkOpengl() = default;
        BenchmarkOpengl(const std::string &testName, GLFWwindow* window, shimera::IBackend* backend, shimera::EffectPipeline &&pipeline, GLint vramUsed);
        ~BenchmarkOpengl() override = default;

        std::string getName() const override;
        void run() override;
        void setupScene(BenchmarkReport &report);
        void renderScene(float &time, float &r);

    private:
        std::string m_name;
        GLFWwindow* m_window;
        shimera::EffectPipeline m_pipeline;
        unsigned int m_vao;
        unsigned int m_shader;
        unsigned int m_ibo;
        unsigned int m_buffer;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::Uniform<shimera::Vec4<float>>* m_colorUniform;
        GLint m_vramUsed;
};

#endif //SHIMERA_BENCHMARKOPENGL_HPP