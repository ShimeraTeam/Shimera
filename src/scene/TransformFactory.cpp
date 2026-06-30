#include "scene/TransformFactory.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "converts/GlmConvert.hpp"


shimera::Mat4 shimera::TransformFactory::model(const Vec3<float>& position, const Vec3<float>& rotationEuler,
    const Vec3<float>& scale) {
    glm::mat4 m(1.0f);
    m = glm::translate(m, toGlmVec3(position));
    m = glm::rotate(m, glm::radians(rotationEuler.z), {0.0f, 0.0f, 1.0f});
    m = glm::rotate(m, glm::radians(rotationEuler.y), {0.0f, 1.0f, 0.0f});
    m = glm::rotate(m, glm::radians(rotationEuler.x), {1.0f, 0.0f, 0.0f});
    m = glm::scale(m, toGlmVec3(scale));
    return toShimMat4(m);
}
