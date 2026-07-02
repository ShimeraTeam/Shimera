#include "scene/CameraFactory.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "converts/GlmConvert.hpp"

shimera::Camera shimera::CameraFactory::perspective(const Vec3<float>& position, const Vec3<float>& target,
                                                    const Vec3<float>& up, float fovYDegrees, float aspect, float nearPlane, float farPlane) {
    const glm::mat4 view = glm::lookAt(toGlmVec3(position), toGlmVec3(target), toGlmVec3(up));
    const glm::mat4 proj = glm::perspective(glm::radians(fovYDegrees), aspect, nearPlane, farPlane);

    Camera cam;
    cam.view = toShimMat4(view);
    cam.projection = toShimMat4(proj);
    cam.position = position;
    return cam;
}
