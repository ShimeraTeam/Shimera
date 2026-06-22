#ifndef SHIMERA_BENCHMARKRAYLIB_HPP
#define SHIMERA_BENCHMARKRAYLIB_HPP

#include "raylib.h"
#include <iostream>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "../../BenchmarkReport.hpp"

class BenchmarkRaylib : public ITest {
public:
    BenchmarkRaylib() = default;
    BenchmarkRaylib(const std::string &testName);
    ~BenchmarkRaylib() override = default;

    std::string getName() const override;
    void run() override;
    void setupScene(BenchmarkReport &report);
    void renderScene(float &time);

    private:
        std::string m_name;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::DistortionEffect* m_distortionEffect;
        Vector3 m_cubePosition;
        Camera m_camera;
};

#endif //SHIMERA_BENCHMARKRAYLIB_HPP