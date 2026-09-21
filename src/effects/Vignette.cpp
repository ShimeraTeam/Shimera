#include "Vignette.hpp"

#include "Resources.inl"

using shimera::Vignette;

Vignette::Vignette(const float strength, const float radius, const float gap,
                   const Vec4<float> color, const bool rounded)
    : m_strength(strength), m_radius(radius), m_gap(gap),
      m_color(color), m_rounded(rounded ? 1 : 0) {
    // The only diff between Slang shader and hand written shaders.
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("vignette.frag"));
}

void Vignette::updateUniforms() {
    m_processor->setUniform("u_strength", m_strength);
    m_processor->setUniform("u_radius", m_radius);
    m_processor->setUniform("u_gap", m_gap);
    m_processor->setUniform("u_color", m_color);
    m_processor->setUniform("u_isRounded", m_rounded);
    m_processor->setUniform("u_resolution", m_resolution);
}

Vignette& Vignette::withStrength(const float strength) {
    m_strength = strength;
    return *this;
}

Vignette& Vignette::withRadius(const float radius) {
    m_radius = radius;
    return *this;
}

Vignette& Vignette::withGap(const float gap) {
    m_gap = gap;
    return *this;
}

Vignette& Vignette::withColor(const Vec4<float> color) {
    m_color = color;
    return *this;
}

Vignette& Vignette::withRounded(const bool rounded) {
    m_rounded = rounded ? 1 : 0;
    return *this;
}
