#include "effects/ContrastEffect.hpp"

ContrastEffect::ContrastEffect(IBackend *backend)
    : ContrastEffect(backend, 1.0f) {}

ContrastEffect::ContrastEffect(IBackend *backend, float u_contrastFactor)
    : u_contrastFactor(u_contrastFactor) {
     m_processor = std::unique_ptr<IPostProccessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/contrast.frag"
            )
        );
}

void ContrastEffect::updateUniforms() {
    m_processor->setUniform("u_contrastFactor", u_contrastFactor);
}

ContrastEffect &ContrastEffect::withContrastFactor(float u_contrastFactor) {
    this->u_contrastFactor = u_contrastFactor;
    return *this;
}