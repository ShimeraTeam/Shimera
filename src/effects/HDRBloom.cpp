#include "HDRBloom.hpp"

#include "Resources.inl"

using shimera::GLTexture;
using shimera::HDRBloom;

HDRBloom::HDRBloom(const float threshold, const float knee, const float intensity,
                   const float blurSigma, const int blurSamples)
    : m_threshold(threshold), m_knee(knee), m_intensity(intensity),
      m_blurSigma(blurSigma), m_blurSamples(blurSamples) {
    m_processor = std::make_unique<GLPostProcessor>(postProcessVertex(), builtinShader("hdr_bloom.frag"));
}

void HDRBloom::resize(const int width, const int height) {
    Effect::resize(width, height);
    if (m_brightBlurredH)
        m_brightBlurredH->resize(width, height);
    else
        m_brightBlurredH = std::make_unique<GLFramebuffer>(width, height);
}

void HDRBloom::render(const GLTexture& input) {
    if (!m_enabled)
        return;
    // Pass one hijacks the bound target, so remember where the caller wanted the result.
    renderImpl(input, currentDrawFramebuffer());
}

void HDRBloom::render(const GLTexture& input, GLFramebuffer& target) {
    if (!m_enabled)
        return;
    renderImpl(input, target.handle());
}

void HDRBloom::renderImpl(const GLTexture& input, const uint32_t target) {
    updateUniforms();

    // Pass 1. bright-pass + horizontal blur into the intermediate.
    m_processor->setUniform("u_mode", 0);
    m_brightBlurredH->bind();
    m_processor->render(input);

    // Pass 2. vertical blur of that, composited over the original scene. (I still don't get why that's necessary)
    m_processor->setUniform("u_mode", 1);
    m_processor->addInputTexture("u_brightBlurredH", m_brightBlurredH->color(), 1);
    bindFramebuffer(target);
    m_processor->render(input);
}

void HDRBloom::updateUniforms() {
    m_processor->setUniform("u_threshold", m_threshold);
    m_processor->setUniform("u_knee", m_knee);
    m_processor->setUniform("u_intensity", m_intensity);
    m_processor->setUniform("u_sigma", m_blurSigma);
    m_processor->setUniform("u_samples", m_blurSamples);
    m_processor->setUniform("u_resolution", m_resolution);
}

HDRBloom& HDRBloom::withThreshold(const float threshold) {
    m_threshold = threshold;
    return *this;
}

HDRBloom& HDRBloom::withKnee(const float knee) {
    m_knee = knee;
    return *this;
}

HDRBloom& HDRBloom::withIntensity(const float intensity) {
    m_intensity = intensity;
    return *this;
}

HDRBloom& HDRBloom::withBlurSigma(const float sigma) {
    m_blurSigma = sigma;
    return *this;
}

HDRBloom& HDRBloom::withBlurSamples(const int samples) {
    m_blurSamples = samples;
    return *this;
}
