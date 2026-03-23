#include "effects/ColortintEffect.hpp"

ColortintEffect::ColortintEffect(IBackend *backend)
    : ColortintEffect(backend, Vec3<float>(0.0f)) {}

ColortintEffect::ColortintEffect(IBackend *backend, Vec3<float> tint)
    : m_uTint(tint) {
     m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/colortint.frag"
        )
     );
}

void ColortintEffect::updateUniforms() {
    m_processor->setUniform("u_tint", m_uTint);
}

ColortintEffect &ColortintEffect::withTint(const Vec3<float> &tint) {
    this->m_uTint = tint;
    return *this;
}