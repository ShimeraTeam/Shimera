#include "effects/ChromaticAberration.hpp"


ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend)
    : ChromaticAberrationEffect(backend, Vec2(0.005f, 0.005f), 0.5f) {}

ChromaticAberrationEffect::ChromaticAberrationEffect(IBackend* backend, const Vec2<float> offset, const float radius)
    : offset(offset), radius(radius) {
     m_processor = std::unique_ptr<IPostProccessor>(
        backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/chromatic_aberration.frag"
        )
     );
}

void ChromaticAberrationEffect::updateUniforms() {
    m_processor->setUniform("u_offset", offset);
    m_processor->setUniform("u_radius", radius);
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withOffset(Vec2<float> o) {
    offset = o;
    return *this;
}

ChromaticAberrationEffect& ChromaticAberrationEffect::withRadius(float r) {
    radius = r;
    return *this;
}
