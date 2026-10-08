#pragma once

#include <variant>

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Mat4.hpp"

namespace shimera {

/* Anything an effect can hand to a shader.
 *
 * Split out of Uniform.inl so public headers can name it without dragging in GL. */
using UniformValue = std::variant<float, int, Vec2<float>, Vec3<float>, Vec4<float>, Mat4>;

}
