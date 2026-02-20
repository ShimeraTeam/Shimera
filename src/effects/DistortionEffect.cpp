#include "effects/DistortionEffect.hpp"


DistortionEffect::DistortionEffect(IBackend* backend)
    : DistortionEffect(backend, 0.0f, 3.0f, 0.13f, 0.1f) {}

DistortionEffect::DistortionEffect(IBackend* backend, const float time, const float noiseScale,
    const float distortionStrength, const float timeScale)
    : time(time), noiseScale(noiseScale), distortionStrength(distortionStrength), timeScale(timeScale) {
     m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/distortion.frag"
        )
     );

}

void DistortionEffect::updateUniforms() {
    m_processor->setUniform("time", time);
    m_processor->setUniform("noiseScale", noiseScale);
    m_processor->setUniform("distortionStrength", distortionStrength);
    m_processor->setUniform("timeScale", timeScale);
}

DistortionEffect& DistortionEffect::withTime(float t) {
    time = t;
    return *this;
}

DistortionEffect& DistortionEffect::withNoiseScale(float s) {
    noiseScale = s;
    return *this;
}

DistortionEffect& DistortionEffect::withDistortionStrength(float s) {
    distortionStrength = s;
    return *this;
}

DistortionEffect& DistortionEffect::withTimeScale(float s) {
    timeScale = s;
    return *this;
}
