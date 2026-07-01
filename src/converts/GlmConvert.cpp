#include "converts/GlmConvert.hpp"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

namespace shimera {

Mat4 toShimMat4(const glm::mat4& mat) {
    Mat4 result{};
    std::memcpy(result.m, glm::value_ptr(mat), 16 * sizeof(float));
    return result;
}

glm::vec3 toGlmVec3(const Vec3<float>& v) {
    return {v.x, v.y, v.z};
}

}
