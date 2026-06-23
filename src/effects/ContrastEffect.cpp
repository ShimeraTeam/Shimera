#include "effects/ContrastEffect.hpp"

using shimera::ContrastEffect;
using shimera::IBackend;
using shimera::IPostProcessor;

ContrastEffect::ContrastEffect(IBackend *backend)
    : ContrastEffect(backend, 1.0f) {}

ContrastEffect::ContrastEffect(IBackend *backend, float u_contrastFactor)
    : m_uContrastFactor(u_contrastFactor) {
     m_processor = std::unique_ptr<IPostProcessor>(
            backend->createPostProcessor(
                "../../../../res/shader/postprocessing/postprocess.vert",
                "../../../../res/shader/postprocessing/contrast.frag"
            )
        );
}

void ContrastEffect::updateUniforms() {
    m_processor->setUniform("u_contrastFactor", m_uContrastFactor);
}

ContrastEffect &ContrastEffect::withContrastFactor(float u_contrastFactor) {
    this->m_uContrastFactor = u_contrastFactor;
    return *this;
}

std::string ContrastEffect::getName() const {
    return "ContrastEffect";
}