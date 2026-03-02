#include "effects/ColorshiftEffect.hpp"

ColorshiftEffect::ColorshiftEffect(IBackend *backend)
    : ColorshiftEffect(backend, Vec3<float>(0.0f)) {}

ColorshiftEffect::ColorshiftEffect(IBackend *backend, Vec3<float> tint)
    : u_tint(tint) {
     m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/colorshift.frag"
        )
     );
}

void ColorshiftEffect::updateUniforms() {
    m_processor->setUniform("u_tint", u_tint);
}

ColorshiftEffect &ColorshiftEffect::withTint(const Vec3<float> &tint) {
    this->u_tint = tint;
    return *this;
}