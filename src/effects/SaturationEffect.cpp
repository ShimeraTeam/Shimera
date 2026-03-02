#include "effects/SaturationEffect.hpp"

SaturationEffect::SaturationEffect(IBackend *backend)
    : SaturationEffect(backend, 1.0f) {}

SaturationEffect::SaturationEffect(IBackend *backend, float u_strength)
    : u_strength(u_strength) {
     m_processor = std::unique_ptr<IPostProccessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/saturation.frag"
            )
        );
}

void SaturationEffect::updateUniforms() {
    m_processor->setUniform("u_strength", u_strength);
}

SaturationEffect &SaturationEffect::withSaturation(float u_strength) {
    this->u_strength = u_strength;
    return *this;
}