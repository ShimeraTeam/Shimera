#include "effects/ChromaticAberration.hpp"


ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend)
    : ChromaticAberrationEffect(backend, 1.0f, false, 2.0f, 20) {}

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend, const float strength, const bool radius,
    const float contrast, const int samples)
    : strength(strength), radius(radius), contrast(contrast), samples(samples) {
     m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/chromatic_aberration.frag"
        )
     );
}

void ChromaticAberrationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", strength);
    m_processor->setUniform("u_radius", radius ? 1 : 0);
    m_processor->setUniform("u_contrast", contrast);
        m_processor->setUniform("u_samples", samples);
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withStrength(float s) {
    strength = s;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withRadius(bool r) {
    radius = r;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withContrast(float c) {
    contrast = c;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withSamples(int s) {
    samples = s;
    return *this;
}
