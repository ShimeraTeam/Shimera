#include "effects/DistortionEffect.hpp"


DistortionEffect::DistortionEffect(IBackend* backend)
    : DistortionEffect(backend, 0.0f, 3.0f, 0.13f, 0.1f) {}

DistortionEffect::DistortionEffect(IBackend* backend, const float time, const float noiseScale,
    const float distortionStrength, const float timeScale)
    : m_uTime(time), m_uNoiseScale(noiseScale), m_uDistortionStrength(distortionStrength), m_uTimeScale(timeScale) {
     m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/distortion.frag"
        )
     );

}

void DistortionEffect::updateUniforms() {
    m_processor->setUniform("time", m_uTime);
    m_processor->setUniform("noiseScale", m_uNoiseScale);
    m_processor->setUniform("distortionStrength", m_uDistortionStrength);
    m_processor->setUniform("timeScale", m_uTimeScale);
}

DistortionEffect& DistortionEffect::withTime(float t) {
    m_uTime = t;
    return *this;
}

DistortionEffect& DistortionEffect::withNoiseScale(float s) {
    m_uNoiseScale = s;
    return *this;
}

DistortionEffect& DistortionEffect::withDistortionStrength(float s) {
    m_uDistortionStrength = s;
    return *this;
}

DistortionEffect& DistortionEffect::withTimeScale(float s) {
    m_uTimeScale = s;
    return *this;
}
