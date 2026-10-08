#include "Transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

using shimera::Mat4;
using shimera::Transform;

Mat4 Transform::toMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
    model = glm::rotate(model, glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

    Mat4 result{};
    std::copy_n(glm::value_ptr(model), 16, result.m);
    return result;
}
