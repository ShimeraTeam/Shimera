#include "effects/ColorshiftEffect.hpp"

ColorshiftEffect::ColorshiftEffect(IBackend *backend)
    : ColorshiftEffect(backend, Vec3<float>(0.0f)) {}

ColorshiftEffect::ColorshiftEffect(IBackend *backend, Vec3<float> colorShift)
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

ColorshiftEffect &ColorshiftEffect::withColorShift(const Vec3<float> &colorShift) {
    this->colorShift = colorShift;
    return *this;
}