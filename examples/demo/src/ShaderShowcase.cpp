#include "ShaderShowcase.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "effects/HDRBloomEffect.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/VignetteEffect.hpp"
#include "effects/PixelisationEffect.hpp"
#include "effects/GaussianBlurEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ContrastEffect.hpp"
#include "effects/ColortintEffect.hpp"
#include "effects/AtmosphericScatteringEffect.hpp"

#include "DemoConfig.hpp"
#include "MathUtils.hpp"

namespace demo {
namespace {

// Runs `fn` on an effect only when this run actually put it in the pipeline.
template<typename TEffect, typename Fn>
void updateIfActive(shimera::EffectPipeline &pipeline, Fn &&fn)
{
    try {
        fn(pipeline.get<TEffect>());
    } catch (const std::out_of_range &) {
        // Effect not in the pipeline this run, nothing to update
    }
}

// --- Editable parameters, one overload per effect ------------------------------
// Only the plain settings are listed: per-frame inputs (time, camera, planet)
// and things like withResolution() stay out of the UI. Initial values mirror
// the configuration done in the constructor below; slider ranges are picked
// to keep the effect readable.

using Vec3f = shimera::Vec3<float>;

void describeParams(ShaderToggle &t, shimera::HDRBloomEffect &fx)
{
    t.params = {
        ShaderParam::number("Threshold", 0.95f, 0.0f, 2.0f, [&fx](float v) { fx.withThreshold(v); }),
        ShaderParam::number("Knee", 0.2f, 0.0f, 1.0f, [&fx](float v) { fx.withKnee(v); }),
        ShaderParam::number("Intensity", 1.3f, 0.0f, 5.0f, [&fx](float v) { fx.withIntensity(v); }),
        ShaderParam::number("Blur sigma", 20.0f, 1.0f, 50.0f, [&fx](float v) { fx.withBlurSigma(v); }),
        ShaderParam::integer("Blur samples", 60, 1, 128, [&fx](int v) { fx.withBlurSamples(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::ChromaticAberrationEffect &fx)
{
    t.params = {
        ShaderParam::number("Strength", 0.7f, 0.0f, 3.0f, [&fx](float v) { fx.withStrength(v); }),
        ShaderParam::boolean("Radius", true, [&fx](bool v) { fx.withRadius(v); }),
        ShaderParam::number("Contrast", 2.0f, 0.0f, 5.0f, [&fx](float v) { fx.withContrast(v); }),
        ShaderParam::integer("Samples", 16, 1, 64, [&fx](int v) { fx.withSamples(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::VignetteEffect &fx)
{
    t.params = {
        ShaderParam::number("Strength", 1.2f, 0.0f, 3.0f, [&fx](float v) { fx.withStrength(v); }),
        ShaderParam::number("Radius", 0.5f, 0.0f, 1.0f, [&fx](float v) { fx.withRadius(v); }),
        ShaderParam::number("Gap", 0.4f, 0.0f, 1.0f, [&fx](float v) { fx.withGap(v); }),
        ShaderParam::boolean("IsRounded", true, [&fx](bool v) { fx.withIsRounded(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::PixelisationEffect &fx)
{
    t.params = {
        ShaderParam::number("PixelSizeX", 16.0f, 1.0f, 64.0f, [&fx](float v) { fx.withPixelSizeX(v); }),
        ShaderParam::number("PixelSizeY", 16.0f, 1.0f, 64.0f, [&fx](float v) { fx.withPixelSizeY(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::GaussianBlurEffect &fx)
{
    t.params = {
        ShaderParam::number("Sigma", 6.0f, 0.1f, 30.0f, [&fx](float v) { fx.withSigma(v); }),
        ShaderParam::integer("Samples", 20, 1, 64, [&fx](int v) { fx.withSamples(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::DistortionEffect &fx)
{
    t.params = {
        ShaderParam::number("NoiseScale", 2.0f, 0.1f, 10.0f, [&fx](float v) { fx.withNoiseScale(v); }),
        ShaderParam::number("DistortionStrength", 0.04f, 0.0f, 0.3f,
                            [&fx](float v) { fx.withDistortionStrength(v); }),
        ShaderParam::number("TimeScale", 0.3f, 0.0f, 3.0f, [&fx](float v) { fx.withTimeScale(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::ContrastEffect &fx)
{
    t.params = {
        ShaderParam::number("ContrastFactor", 1.6f, 0.0f, 4.0f, [&fx](float v) { fx.withContrastFactor(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::ColortintEffect &fx)
{
    t.params = {
        ShaderParam::vec3("Tint", Vec3f(1.0f, 0.5f, 0.15f), [&fx](Vec3f v) { fx.withTint(v); }),
    };
}

void describeParams(ShaderToggle &t, shimera::AtmosphericScatteringEffect &fx)
{
    // withQuality() takes both counts at once: the two sliders share them.
    auto quality = std::make_shared<std::pair<int, int>>(16, 16);
    t.params = {
        ShaderParam::vec3("Sun", Vec3f(100.0f, 100.0f, 0.0f), [&fx](Vec3f v) { fx.withSun(v); }),
        ShaderParam::integer("OpticalDepthSamples", 16, 1, 64, [&fx, quality](int v) {
            quality->first = v;
            fx.withQuality(quality->first, quality->second);
        }),
        ShaderParam::integer("InScatteringPoints", 16, 1, 64, [&fx, quality](int v) {
            quality->second = v;
            fx.withQuality(quality->first, quality->second);
        }),
    };
}

void describeParams(ShaderToggle &t, shimera::FresnelEffect &fx)
{
    t.params = {
        ShaderParam::vec3("Color", Vec3f(0.3f, 0.7f, 1.0f), [&fx](Vec3f v) { fx.withColor(v); }),
        ShaderParam::number("Power", 3.0f, 0.1f, 10.0f, [&fx](float v) { fx.withPower(v); }),
        ShaderParam::number("Reflectance", 0.04f, 0.0f, 1.0f, [&fx](float v) { fx.withReflectance(v); }),
        ShaderParam::number("Intensity", 1.5f, 0.0f, 5.0f, [&fx](float v) { fx.withIntensity(v); }),
    };
}

}

template<typename TEffect>
void ShaderShowcase::addToggleableEffect(TEffect &&effect, std::string label)
{
    m_pipeline.addEffect(std::move(effect));
    // The pipeline owns the effect from here on and never moves it, so the
    // parameter setters can safely keep a reference to it.
    auto &added = m_pipeline.get<std::decay_t<TEffect>>();
    added.setEnabled(false);
    m_toggles.push_back({std::move(label), false, &added, {}});
    describeParams(m_toggles.back(), added);
}

ShaderShowcase::ShaderShowcase(shimera::IBackend *backend)
    : m_backend(backend),
      m_pipeline(backend, kWidth, kHeight),
      m_fresnelModel(LoadModelFromMesh(GenMeshSphere(1.1f, 32, 32))),
      m_fresnelMesh(m_fresnelModel),
      m_fresnel(backend)
{
    const auto resolution = shimera::Vec2(static_cast<float>(kWidth), static_cast<float>(kHeight));

    // 1. Create and configure the effects. Every parameter is set through a
    // fluent .withX() call. No shader code, no GPU knowledge required.
    shimera::HDRBloomEffect bloom(m_backend);
    bloom.withThreshold(0.95f)
         .withKnee(0.2f)
         .withIntensity(1.3f)
         .withBlurSigma(20.0f)
         .withBlurSamples(60)
         .withResolution(resolution);

    shimera::ChromaticAberrationEffect chroma(m_backend);
    chroma.withStrength(0.7f)
          .withRadius(true)
          .withContrast(2.0f)
          .withSamples(16);

    shimera::VignetteEffect vignette(m_backend);
    vignette.withStrength(1.2f)
            .withRadius(0.5f)
            .withGap(0.4f)
            .withIsRounded(true)
            .withResolution(resolution);

    shimera::PixelisationEffect pixelise(m_backend);
    pixelise.withPixelSizeX(16.0f)
            .withPixelSizeY(16.0f)
            .withResolution(resolution);

    shimera::GaussianBlurEffect blur(m_backend);
    blur.withSigma(6.0f)
        .withSamples(20)
        .withResolution(resolution);

    shimera::DistortionEffect distortion(m_backend);
    distortion.withNoiseScale(2.0f)
              .withDistortionStrength(0.04f)
              .withTimeScale(0.3f);

    shimera::ContrastEffect contrast(m_backend);
    contrast.withContrastFactor(1.6f);

    shimera::ColortintEffect colortint(m_backend);
    colortint.withTint(shimera::Vec3(1.0f, 0.5f, 0.15f));

    shimera::AtmosphericScatteringEffect atmosphere(m_backend);
    atmosphere.withSun(shimera::Vec3(100.0f, 100.0f, 0.0f))
              .withQuality(16, 16);

    // A 3D material effect, drawn with the scene instead of the pipeline.
    m_fresnel.withColor(shimera::Vec3(0.3f, 0.7f, 1.0f))
             .withPower(3.0f)
             .withReflectance(0.04f)
             .withIntensity(1.5f);
    m_fresnel.setTransform(shimera::Vec3(-1.8f, 7.1f, -1.5f));

    // 2. Assemble the post-processing chain.
    if constexpr (kUseShaderMenu) {
        // Menu mode: every effect goes in the chain (in this order) but starts
        // disabled; the checkboxes only flip setEnabled() at runtime.
        addToggleableEffect(std::move(bloom), "Bloom HDR");
        addToggleableEffect(std::move(chroma), "Aberration chromatique");
        addToggleableEffect(std::move(vignette), "Vignettage");
        addToggleableEffect(std::move(pixelise), "Pixelisation");
        addToggleableEffect(std::move(blur), "Flou gaussien");
        addToggleableEffect(std::move(distortion), "Distorsion");
        addToggleableEffect(std::move(contrast), "Contraste");
        addToggleableEffect(std::move(colortint), "Teinte chaude");
        addToggleableEffect(std::move(atmosphere), "Diffusion atmospherique");

        // The Fresnel material is not a post-process, so its checkbox drives a
        // plain flag instead of the pipeline.
        m_fresnelToggleIndex = m_toggles.size();
        m_toggles.push_back({"Fresnel (materiau 3D)", m_fresnelEnabled, nullptr, {}});
        describeParams(m_toggles.back(), m_fresnel);
    } else {
        // Manual mode: uncomment a line to enable that effect.
        // m_pipeline.addEffect(std::move(bloom));        // HDR bloom: glow on bright areas
        // m_pipeline.addEffect(std::move(chroma));       // chromatic aberration
        // m_pipeline.addEffect(std::move(vignette));     // darkened edges
        // m_pipeline.addEffect(std::move(pixelise));     // retro pixelation
        // m_pipeline.addEffect(std::move(blur));         // gaussian blur
        // m_pipeline.addEffect(std::move(distortion));   // animated screen distortion
        // m_pipeline.addEffect(std::move(contrast));     // contrast boost
        // m_pipeline.addEffect(std::move(colortint));    // warm color tint
        // m_pipeline.addEffect(std::move(atmosphere));   // atmospheric scattering on the sphere
    }

    // Seulement quand la pipeline est vide
    m_passthrough.reset(m_backend->createPostProcessor(
        "../../../../res/shader/postprocessing/postprocess.vert",
        "../../../../res/shader/postprocessing/normal.frag"));
}

ShaderShowcase::~ShaderShowcase()
{
    UnloadModel(m_fresnelModel);
}

void ShaderShowcase::applyToggles()
{
    if constexpr (!kUseShaderMenu)
        return;

    for (auto &toggle : m_toggles) {
        if (toggle.effect)
            toggle.effect->setEnabled(toggle.enabled);
    }
    m_fresnelEnabled = m_toggles[m_fresnelToggleIndex].enabled;
}

void ShaderShowcase::updateUniforms(float time, const Camera &camera)
{
    updateIfActive<shimera::DistortionEffect>(m_pipeline, [time](auto &fx) {
        fx.withTime(time);
    });
    updateIfActive<shimera::AtmosphericScatteringEffect>(m_pipeline, [&camera](auto &fx) {
        constexpr float aspect = static_cast<float>(kWidth) / static_cast<float>(kHeight);
        fx.withCamera(toVec3(camera.position), toVec3(camera.target),
                      camera.fovy, aspect, kNearPlane, kFarPlane)
          .withPlanet(toVec3(kSphereCenter), kSphereRadius,
                      kSphereRadius * kAtmosphereScale);
    });
}

void ShaderShowcase::renderMaterials(const shimera::Camera &camera)
{
    if (m_fresnelEnabled)
        m_fresnel.render(m_fresnelMesh, camera);
}

void ShaderShowcase::render(shimera::IFrameBuffer &sceneFb)
{
    // In menu mode the pipeline is never empty, only its enabled effects
    // matter: with none of them on, the chain would render nothing at all.
    if (activeEffectCount() == 0)
        m_passthrough->render(sceneFb.getTexture());
    else
        m_pipeline.render(sceneFb.getTexture(), &sceneFb.getDepthTexture());
}

std::size_t ShaderShowcase::activeEffectCount() const
{
    if constexpr (!kUseShaderMenu)
        return m_pipeline.size();

    std::size_t count = 0;
    for (const auto &toggle : m_toggles) {
        if (toggle.effect && toggle.enabled)
            ++count;
    }
    return count;
}

std::string ShaderShowcase::activeEffectNames() const
{
    if constexpr (!kUseShaderMenu)
        return m_pipeline.getEffectsNames();

    std::string names;
    for (const auto &toggle : m_toggles) {
        if (toggle.effect && toggle.enabled)
            names += toggle.name + ", ";
    }
    if (names.empty())
        return "aucun";
    names.resize(names.size() - 2);
    return names;
}

}
