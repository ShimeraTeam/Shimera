#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

using shimera::Camera;

namespace {

shimera::Mat4 toMat4(const glm::mat4& source) {
    shimera::Mat4 result{};
    const float* values = glm::value_ptr(source);
    std::copy_n(values, 16, result.m);
    return result;
}

glm::vec3 toGlm(const shimera::Vec3<float>& v) {
    return {v.x, v.y, v.z};
}

}

Camera Camera::fromMatrices(const Mat4& view, const Mat4& projection, const Vec3<float>& position) {
    Camera camera;
    camera.view = view;
    camera.projection = projection;
    camera.position = position;
    return camera;
}

Camera Camera::perspective(const Vec3<float>& position,
                           const Vec3<float>& target,
                           const Vec3<float>& up,
                           const float fovYDegrees,
                           const float aspect,
                           const float nearPlane,
                           const float farPlane) {
    Camera camera;
    camera.position = position;
    camera.view = toMat4(glm::lookAt(toGlm(position), toGlm(target), toGlm(up)));
    camera.projection = toMat4(glm::perspective(glm::radians(fovYDegrees),
        aspect > 0.0f ? aspect : 1.0f, nearPlane, farPlane));
    return camera;
}
