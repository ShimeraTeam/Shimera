#include "effects/GrayscaleEffect.hpp"

GrayscaleEffect::GrayscaleEffect(IBackend *backend) {
    m_processor = std::unique_ptr<IPostProcessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/saturation.frag"
        )
    );
}

void GrayscaleEffect::updateUniforms() {
    m_processor->setUniform("u_strength", 0.0f);
}
