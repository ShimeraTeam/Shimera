#include "effects/ColorshiftEffect.hpp"

ColorshiftEffect::ColorshiftEffect(IBackend *backend)
    : ColorshiftEffect(backend, vec3(0.0f)) {}

ColorshiftEffect::ColorshiftEffect(IBackend *backend, vec3 colorShift)
    : colorShift(colorShift) {
     m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/colorshift.frag"
        )
     );
}

void ColorshiftEffect::updateUniforms() {
    m_processor->setUniform("colorShift", colorShift);
}

ColorshiftEffect &ColorshiftEffect::withColorShift(const vec3 &colorShift) {
    this->colorShift = colorShift;
    return *this;
}