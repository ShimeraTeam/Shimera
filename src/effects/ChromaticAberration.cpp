#include "effects/ChromaticAberration.hpp"

using shimera::ChromaticAberrationEffect;
using shimera::IBackend;
using shimera::IPostProcessor;

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend)
    : ChromaticAberrationEffect(backend, 1.0f, false, 2.0f, 20) {}

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend, const float strength, const bool radius,
    const float contrast, const int samples)
    : m_uStrength(strength), m_uRadius(radius), m_uContrast(contrast), m_uSamples(samples) {
     m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/chromatic_aberration.frag"
        )
     );
}

void ChromaticAberrationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
    m_processor->setUniform("u_radius", m_uRadius ? 1 : 0);
    m_processor->setUniform("u_contrast", m_uContrast);
        m_processor->setUniform("u_samples", m_uSamples);
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withStrength(float s) {
    m_uStrength = s;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withRadius(bool r) {
    m_uRadius = r;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withContrast(float c) {
    m_uContrast = c;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withSamples(int s) {
    m_uSamples = s;
    return *this;
}

std::string ChromaticAberrationEffect::getName() const {
    return "ChromaticAberrationEffect";
}