#include "effects/SaturationEffect.hpp"

SaturationEffect::SaturationEffect(IBackend *backend)
    : SaturationEffect(backend, 1.0f) {}

SaturationEffect::SaturationEffect(IBackend *backend, float saturation)
    : saturation(saturation) {
     m_processor = std::unique_ptr<IPostProccessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/saturation.frag"
            )
        );
}

void SaturationEffect::updateUniforms() {
    m_processor->setUniform("saturation", saturation);
}

SaturationEffect &SaturationEffect::withSaturation(float saturation) {
    this->saturation = saturation;
    return *this;
}