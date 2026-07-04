#include "raylib.h"
#include <iostream>
#include <GL/glew.h>
#include "../../memory/MemoryTest.hpp"
#include "../../TestRunner.hpp"
#include "BenchmarkRaylib.hpp"
#include "effects/GaussianBlurEffect.hpp"
#include "effects/VignetteEffect.hpp"
#include "effects/BrightnessEffect.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/ColortintEffect.hpp"
#include "effects/ContrastEffect.hpp"
#include "effects/GrayscaleEffect.hpp"
#include "effects/SaturationEffect.hpp"
#include "effects/PixelisationEffect.hpp"
#include "effects/HDRBloomEffect.hpp"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

int main() {
    InitWindow(960, 540, "Raylib shimera tests");
    glewInit();
    shimera::IBackend *backend = shimera::BackendFactory::create();
    TestRunner runner;
    GLint vramBefore = 0;
    GLint vramAfter = 0;
    GLint vramUsed = 0;

    // Memory test
    runner.add(std::make_unique<MemoryTest>("Memory Tests"));

    // Benchmark GaussianBlurEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineGaussianBlur(backend, 640, 480);
    pipelineGaussianBlur.addEffect<shimera::GaussianBlurEffect>(5.0f, 15, shimera::Vec2<float>(640.0f, 480.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib GaussianBlurEffect", backend, std::move(pipelineGaussianBlur), vramUsed));

    // Benchmark VignetteEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineVignette(backend, 640, 480);
    pipelineVignette.addEffect<shimera::VignetteEffect>(0.8f, 0.3f, 0.1f, shimera::Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f), true, shimera::Vec2<float>(640.0f, 480.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib VignetteEffect", backend, std::move(pipelineVignette), vramUsed));

    // Benchmark HDRBloom
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineHDRBloom(backend, 640, 480);
    pipelineHDRBloom.addEffect<shimera::HDRBloomEffect>(0.7f, 0.2f, 1.5f, 15.0f, 30, shimera::Vec2<float>(640.0f, 480.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib HDRBloomEffect", backend, std::move(pipelineHDRBloom), vramUsed));

    // Benchmark AtmosphericScatteringEffect
    shimera::EffectPipeline pipelineAtmosphericScattering(backend, 640, 480);
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib AtmosphericScatteringEffect", backend, std::move(pipelineAtmosphericScattering), 0));

    // Benchmark FresnelEffect
    shimera::EffectPipeline pipelineFresnel(backend, 640, 480);
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib FresnelEffect", backend, std::move(pipelineFresnel), 0));

    //Benchmark with two effects
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineTwoEffects(backend, 640, 480);
    pipelineTwoEffects.addEffect<shimera::ContrastEffect>();
    pipelineTwoEffects.addEffect<shimera::SaturationEffect>(0.5f);
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib TwoEffects", backend, std::move(pipelineTwoEffects), vramUsed));

    //Benchmark with three effects
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineThreeEffects(backend, 640, 480);
    pipelineThreeEffects.addEffect<shimera::ContrastEffect>();
    pipelineThreeEffects.addEffect<shimera::GrayscaleEffect>();
    pipelineThreeEffects.addEffect<shimera::GaussianBlurEffect>(5.0f, 15, shimera::Vec2<float>(640.0f, 480.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib ThreeEffects", backend, std::move(pipelineThreeEffects), vramUsed));

    // Benchmark without any effects
    shimera::EffectPipeline pipelineNoEffects(backend, 640, 480);
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib No Effects", backend, std::move(pipelineNoEffects), 0));

    // Benchmark DistortionEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineDistortion(backend, 640, 480);
    pipelineDistortion.addEffect<shimera::DistortionEffect>();
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib DistortionEffect", backend, std::move(pipelineDistortion), vramUsed));

    // Benchmark BrightnessEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineBrightness(backend, 640, 480);
    pipelineBrightness.addEffect<shimera::BrightnessEffect>(0.2f);
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib BrightnessEffect", backend, std::move(pipelineBrightness), vramUsed));

    // Benchmark ChromaticAberration
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineChromaticAberration(backend, 640, 480);
    pipelineChromaticAberration.addEffect<shimera::ChromaticAberrationEffect>(0.5f, false, 2.0f, 20);
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib ChromaticAberration", backend, std::move(pipelineChromaticAberration), vramUsed));

    // Benchmark ColortintEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineColortint(backend, 640, 480);
    pipelineColortint.addEffect<shimera::ColortintEffect>(shimera::Vec3<float>(1.0f, 0.0f, 0.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib ColortintEffect", backend, std::move(pipelineColortint), vramUsed));

    // Benchmark ContrastEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineContrast(backend, 640, 480);
    pipelineContrast.addEffect<shimera::ContrastEffect>(1.5f);
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib ContrastEffect", backend, std::move(pipelineContrast), vramUsed));

    // Benchmark GrayscaleEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineGrayscale(backend, 640, 480);
    pipelineGrayscale.addEffect<shimera::GrayscaleEffect>();
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib GrayscaleEffect", backend, std::move(pipelineGrayscale), vramUsed));

    // Benchmark SaturationEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelineSaturation(backend, 640, 480);
    pipelineSaturation.addEffect<shimera::SaturationEffect>(1.5f);
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib SaturationEffect", backend, std::move(pipelineSaturation), vramUsed ));

    // Benchmark PixelisationEffect
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramBefore);
    shimera::EffectPipeline pipelinePixelisation(backend, 640, 480);
    pipelinePixelisation.addEffect<shimera::PixelisationEffect>(8.0f, shimera::Vec2<float>(640.0f, 480.0f));
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &vramAfter);
    vramUsed = vramBefore - vramAfter;
    runner.add(std::make_unique<BenchmarkRaylib>( "Benchmark Raylib PixelisationEffect", backend, std::move(pipelinePixelisation), vramUsed));

    runner.runAll();
    delete backend;
    CloseWindow();
    exit(0);
}
