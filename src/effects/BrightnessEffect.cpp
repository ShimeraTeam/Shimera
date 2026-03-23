#include "effects/BrightnessEffect.hpp"

BrightnessEffect::BrightnessEffect(IBackend *backend)
    : BrightnessEffect(backend, 0.0f) {}

BrightnessEffect::BrightnessEffect(IBackend *backend, float u_strength)
    : m_uStrength(u_strength) {
     m_processor = std::unique_ptr<IPostProcessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/brightness.frag"
            )
        );
}

void BrightnessEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
}

BrightnessEffect &BrightnessEffect::withStrength(float u_strength) {
    this->m_uStrength = u_strength;
    return *this;
}