#include "GaussianBlur.hpp"

#include "Resources.inl"

using shimera::GaussianBlur;
using shimera::Vec2;

GaussianBlur::GaussianBlur(const float sigma, const int samples, const Vec2<float> resolution)
    : m_sigma(sigma), m_samples(samples), m_uResolution(resolution) {
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"), generatedShader("gaussian_blur.frag"));
}

void GaussianBlur::resize(int width, int height) {
    Effect::resize(width, height);
    if (m_intermediateBuffer)
        m_intermediateBuffer->resize(width, height);
    else
        m_intermediateBuffer = std::make_unique<GLFramebuffer>(width, height);
}

void GaussianBlur::render(const GLTexture& input) {
    if (!m_enabled)
        return;
    renderImpl(input, currentDrawFramebuffer());
}

void GaussianBlur::render(const GLTexture& input, GLFramebuffer& target) {
    if (!m_enabled)
        return;
    renderImpl(input, target.handle());
}

void GaussianBlur::renderImpl(const GLTexture& input, const uint32_t target) {
    updateUniforms();

    m_processor->setUniform("u_direction", Vec2(1.0f, 0.0f));
    m_intermediateBuffer->bind();
    m_processor->render(input);

    m_processor->setUniform("u_direction", Vec2(0.0f, 1.0f));
    bindFramebuffer(target);
    m_processor->render(input);
}

void GaussianBlur::updateUniforms() {
    m_processor->setUniform("u_sigma", m_sigma);
    m_processor->setUniform("u_samples", m_samples);
    m_processor->setUniform("u_resolution", m_uResolution);
}

GaussianBlur& GaussianBlur::withSigma(const float sigma) {
    m_sigma = sigma;
    return *this;
}

GaussianBlur& GaussianBlur::withSamples(const int samples) {
    m_samples = samples;
    return *this;
}

GaussianBlur& GaussianBlur::withResolution(const Vec2<float> resolution) {
    m_uResolution = resolution;
    return *this;
}
