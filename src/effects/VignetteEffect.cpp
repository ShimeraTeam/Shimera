#include "effects/VignetteEffect.hpp"


VignetteEffect::VignetteEffect(IBackend* backend)
    : VignetteEffect(backend, 1.0f, 0.5f, 0.3f, Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f), false, Vec2<float>(1920.0f, 1080.0f)) {}

VignetteEffect::VignetteEffect(IBackend* backend, const float strength, const float radius, const float gap, 
    const Vec4<float> color, const bool isRounded, const Vec2<float> resolution)
    : u_strength(strength), u_radius(radius), u_gap(gap), u_color(color), u_resolution(resolution) {
    u_isRounded = isRounded ? 1 : 0;
    m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/vignette.frag"
        )
    );
}

void VignetteEffect::updateUniforms() {
    m_processor->setUniform("u_strength", u_strength);
    m_processor->setUniform("u_radius", u_radius);
    m_processor->setUniform("u_gap", u_gap);
    m_processor->setUniform("u_color", u_color);
    m_processor->setUniform("u_isRounded", u_isRounded);
    m_processor->setUniform("u_resolution", u_resolution);
}

VignetteEffect& VignetteEffect::withStrength(float strength) {
    u_strength = strength;
    return *this;
}

VignetteEffect& VignetteEffect::withRadius(float radius) {
    u_radius = radius;
    return *this;
}

VignetteEffect& VignetteEffect::withGap(float gap) {
    u_gap = gap;
    return *this;
}

VignetteEffect& VignetteEffect::withColor(Vec4<float> color) {
    u_color = color;
    return *this;
}

VignetteEffect& VignetteEffect::withIsRounded(bool isRounded) {
    u_isRounded = isRounded ? 1 : 0;
    return *this;
}

VignetteEffect& VignetteEffect::withResolution(Vec2<float> resolution) {
    u_resolution = resolution;
    return *this;
}
