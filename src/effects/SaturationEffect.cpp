#include "effects/SaturationEffect.hpp"

using shimera::IBackend;
using shimera::IPostProcessor;
using shimera::SaturationEffect;

SaturationEffect::SaturationEffect(IBackend *backend)
    : SaturationEffect(backend, 1.0f) {}

SaturationEffect::SaturationEffect(IBackend *backend, float u_strength)
    : m_uStrength(u_strength) {
     m_processor = std::unique_ptr<IPostProcessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/saturation.frag"
            )
        );
}

void SaturationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", m_uStrength);
}

SaturationEffect &SaturationEffect::withStrength(float u_strength) {
    this->m_uStrength = u_strength;
    return *this;
}

std::string SaturationEffect::getName() const {
    return "SaturationEffect";
}
