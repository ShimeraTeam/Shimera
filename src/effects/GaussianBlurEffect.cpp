#include "effects/GaussianBlurEffect.hpp"

using shimera::IBackend;
using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::ITexture;
using shimera::Vec2;
using shimera::GaussianBlurEffect;

GaussianBlurEffect::GaussianBlurEffect(IBackend *backend)
    : GaussianBlurEffect(backend, 3.0f, 10, Vec2<float>(1920.0f, 1080.0f)) {}

GaussianBlurEffect::GaussianBlurEffect(IBackend *backend, const float sigma, const int samples, const Vec2<float> resolution)
    : m_uSigma(sigma), m_uSamples(samples), m_uResolution(resolution) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/gaussian_blur.frag"
        )
    );
    m_intermediateBuffer = std::unique_ptr<IFrameBuffer>(
        backend->createFrameBuffer(static_cast<int>(resolution.x), static_cast<int>(resolution.y))
    );
}

void GaussianBlurEffect::render(ITexture &texture) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_direction", Vec2(1.0f, 0.0f));
    m_intermediateBuffer->bind();
    m_processor->render(texture);
    m_intermediateBuffer->unbind();

    m_processor->setUniform("u_direction", Vec2(0.0f, 1.0f));
    m_processor->render(m_intermediateBuffer->getTexture());
}

void GaussianBlurEffect::render(ITexture &texture, IFrameBuffer &target) {
    if (!m_enabled) return;
    updateUniforms();

    m_processor->setUniform("u_direction", Vec2(1.0f, 0.0f));
    m_intermediateBuffer->bind();
    m_processor->render(texture);
    m_intermediateBuffer->unbind();

    m_processor->setUniform("u_direction", Vec2(0.0f, 1.0f));
    target.bind();
    m_processor->render(m_intermediateBuffer->getTexture());
    target.unbind();
}

void GaussianBlurEffect::updateUniforms() {
    m_processor->setUniform("u_sigma", m_uSigma);
    m_processor->setUniform("u_samples", m_uSamples);
    m_processor->setUniform("u_resolution", m_uResolution);
}

GaussianBlurEffect &GaussianBlurEffect::withSigma(float sigma) {
    m_uSigma = sigma;
    return *this;
}

GaussianBlurEffect &GaussianBlurEffect::withSamples(int samples) {
    m_uSamples = samples;
    return *this;
}

GaussianBlurEffect &GaussianBlurEffect::withResolution(Vec2<float> resolution) {
    m_uResolution = resolution;
    m_intermediateBuffer->resize(static_cast<int>(resolution.x), static_cast<int>(resolution.y));
    return *this;
}

std::string GaussianBlurEffect::getName() const {
    return "GaussianBlurEffect";
}
