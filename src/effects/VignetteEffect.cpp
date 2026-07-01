#include "effects/VignetteEffect.hpp"

using shimera::IBackend;
using shimera::IPostProcessor;
using shimera::Vec2;
using shimera::Vec4;
using shimera::VignetteEffect;


VignetteEffect::VignetteEffect(IBackend* backend)
    : VignetteEffect(backend, 1.0f, 0.5f, 0.3f, Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f), false, Vec2<float>(1920.0f, 1080.0f)) {}

VignetteEffect::VignetteEffect(IBackend* backend, const float strength, const float radius, const float gap, 
    const Vec4<float> color, const bool isRounded, const Vec2<float> resolution)
    : m_uStrength(strength), m_uRadius(radius), m_uGap(gap), m_uColor(color), m_uResolution(resolution) {
    m_uIsRounded = isRounded ? 1 : 0;
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/vignette.frag"
        )
    );
}

void VignetteEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
    m_processor->setUniform("u_radius", m_uRadius);
    m_processor->setUniform("u_gap", m_uGap);
    m_processor->setUniform("u_color", m_uColor);
    m_processor->setUniform("u_isRounded", m_uIsRounded);
    m_processor->setUniform("u_resolution", m_uResolution);
}

VignetteEffect& VignetteEffect::withStrength(float strength) {
    m_uStrength = strength;
    return *this;
}

VignetteEffect& VignetteEffect::withRadius(float radius) {
    m_uRadius = radius;
    return *this;
}

VignetteEffect& VignetteEffect::withGap(float gap) {
    m_uGap = gap;
    return *this;
}

VignetteEffect& VignetteEffect::withColor(Vec4<float> color) {
    m_uColor = color;
    return *this;
}

VignetteEffect& VignetteEffect::withIsRounded(bool isRounded) {
    m_uIsRounded = isRounded ? 1 : 0;
    return *this;
}

VignetteEffect& VignetteEffect::withResolution(Vec2<float> resolution) {
    m_uResolution = resolution;
    return *this;
}

std::string VignetteEffect::getName() const {
    return "VignetteEffect";
}
