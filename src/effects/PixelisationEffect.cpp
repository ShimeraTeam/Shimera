#include "effects/PixelisationEffect.hpp"

using shimera::IBackend;
using shimera::IPostProcessor;
using shimera::Vec2;
using shimera::PixelisationEffect;

PixelisationEffect::PixelisationEffect(IBackend *backend)
    : PixelisationEffect(backend, 4.0f, Vec2<float>(1920.0f, 1080.0f)) {}

PixelisationEffect::PixelisationEffect(IBackend *backend, const float pixelSize, const Vec2<float> resolution)
    : m_uPixelSizeX(pixelSize), m_uPixelSizeY(pixelSize), m_uResolution(resolution) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/pixelisation.frag"
        )
    );
}

void PixelisationEffect::updateUniforms() {
    m_processor->setUniform("u_pixelSizeX", m_uPixelSizeX);
    m_processor->setUniform("u_pixelSizeY", m_uPixelSizeY);
    m_processor->setUniform("u_resolution", m_uResolution);
    m_processor->setUniform("u_offset", m_uOffset);
}

PixelisationEffect &PixelisationEffect::withPixelSize(float pixelSize) {
    m_uPixelSizeX = pixelSize;
    m_uPixelSizeY = pixelSize;
    return *this;
}

PixelisationEffect &PixelisationEffect::withPixelSizeX(float pixelSizeX) {
    m_uPixelSizeX = pixelSizeX;
    return *this;
}

PixelisationEffect &PixelisationEffect::withPixelSizeY(float pixelSizeY) {
    m_uPixelSizeY = pixelSizeY;
    return *this;
}

PixelisationEffect &PixelisationEffect::withResolution(Vec2<float> resolution) {
    m_uResolution = resolution;
    return *this;
}

PixelisationEffect &PixelisationEffect::withOffset(Vec2<float> offset) {
    m_uOffset = offset;
    return *this;
}

std::string PixelisationEffect::getName() const {
    return "PixelisationEffect";
}
