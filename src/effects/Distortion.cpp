#include "Distortion.hpp"

#include "Resources.inl"

using shimera::Distortion;

Distortion::Distortion(const float noiseScale, const float distortionStrength, const float timeScale)
    : m_noiseScale(noiseScale), m_distortionStrength(distortionStrength), m_timeScale(timeScale) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"), generatedShader("distortion.frag"));
}

void Distortion::updateUniforms() {
    // distortion.frag predates the u_ naming convention the other shaders use
    // so names below are what the GLSL actually declares. (We have to change that ofc)
    m_processor->setUniform("time", m_time);
    m_processor->setUniform("noiseScale", m_noiseScale);
    m_processor->setUniform("distortionStrength", m_distortionStrength);
    m_processor->setUniform("timeScale", m_timeScale);
}

Distortion& Distortion::withTime(const float time) {
    m_time = time;
    return *this;
}

Distortion& Distortion::withNoiseScale(const float noiseScale) {
    m_noiseScale = noiseScale;
    return *this;
}

Distortion& Distortion::withDistortionStrength(const float distortionStrength) {
    m_distortionStrength = distortionStrength;
    return *this;
}

Distortion& Distortion::withTimeScale(const float timeScale) {
    m_timeScale = timeScale;
    return *this;
}

float Distortion::getTime() const {
    return m_time;
}

float Distortion::getNoiseScale() const {
    return m_noiseScale;
}

float Distortion::getDistortionStrength() const {
    return m_distortionStrength;
}

float Distortion::getTimeScale() const {
    return m_timeScale;
}
