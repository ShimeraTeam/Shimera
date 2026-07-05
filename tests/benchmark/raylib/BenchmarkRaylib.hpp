#ifndef SHIMERA_BENCHMARKRAYLIB_HPP
#define SHIMERA_BENCHMARKRAYLIB_HPP

#include "raylib.h"
#include <iostream>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "backend/raylib/converts/RaylibCamera.hpp"
#include "backend/raylib/RaylibMesh.hpp"
#include "effects/AtmosphericScatteringEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/materials/FresnelEffect.hpp"
#include "../../BenchmarkReport.hpp"
#include "EffectPipeline.inl"

class BenchmarkRaylib : public ITest {
public:
    BenchmarkRaylib() = default;
    BenchmarkRaylib(const std::string &testName, shimera::IBackend* backend, shimera::EffectPipeline &&pipeline, GLint vramUsed);
    ~BenchmarkRaylib() override = default;

    std::string getName() const override;
    void run() override;
    void setupScene(BenchmarkReport &report);
    void renderScene(float &time);

    private:
        std::string m_name;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::AtmosphericScatteringEffect *m_atmosphericScatteringEffect;
        shimera::FresnelEffect *m_fresnelEffect;
        shimera::EffectPipeline m_pipeline;
        GLint m_vramUsed;
        Vector3 m_cubePosition;
        Camera m_camera;
        shimera::RaylibMesh* m_sphereMesh;
        Model m_model;
};

#endif //SHIMERA_BENCHMARKRAYLIB_HPP