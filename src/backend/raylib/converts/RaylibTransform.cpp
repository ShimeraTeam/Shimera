#include "backend/raylib/converts/RaylibTransform.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "converts/GlmConvert.hpp"

shimera::Mat4 shimera::RaylibTransform::toModel(Vector3 position, Vector3 rotationAxis, float rotationAngle,
    Vector3 scale) {
    glm::mat4 m(1.0f);
    m = glm::translate(m, {position.x, position.y, position.z});
    m = glm::rotate(m, glm::radians(rotationAngle),
                    {rotationAxis.x, rotationAxis.y, rotationAxis.z});
    m = glm::scale(m, {scale.x, scale.y, scale.z});
    return toShimMat4(m);
}
