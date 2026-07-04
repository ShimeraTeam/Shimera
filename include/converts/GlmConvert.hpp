#ifndef SHIMERA_GLMCONVERT_HPP
#define SHIMERA_GLMCONVERT_HPP

#include <glm/glm.hpp>

#include "shimera_api.h"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

SHIMERA_API Mat4 toShimMat4(const glm::mat4& mat);

SHIMERA_API glm::vec3 toGlmVec3(const Vec3<float>& v);

}

#endif //SHIMERA_GLMCONVERT_HPP
