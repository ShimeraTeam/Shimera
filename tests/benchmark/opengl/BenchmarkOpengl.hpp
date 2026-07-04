#ifndef SHIMERA_BENCHMARKOPENGL_HPP
#define SHIMERA_BENCHMARKOPENGL_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/AtmosphericScatteringEffect.hpp"
#include "effects/materials/FresnelEffect.hpp"
#include "../../BenchmarkReport.hpp"
#include "scene/Camera.hpp"
#include "scene/CameraFactory.hpp"
#include "EffectPipeline.inl"
#include "shimera.h"

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
        shimera::IMesh* m_sphere;
        shimera::FresnelEffect* m_fresnelEffect;
        unsigned int m_vao;
        unsigned int m_shader;
        unsigned int m_ibo;
        unsigned int m_buffer;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::Uniform<shimera::Vec4<float>>* m_colorUniform;
        GLint m_vramUsed;
        std::vector<float> m_spPos, m_spNrm;
        std::vector<unsigned int> m_spIdx;

        void make2DCube();
};

#endif //SHIMERA_BENCHMARKOPENGL_HPP