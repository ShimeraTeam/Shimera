#ifndef SHIMERA_DEMO_MATHUTILS_HPP
#define SHIMERA_DEMO_MATHUTILS_HPP

#include <cmath>

#include "raylib.h"
#include "shimera.h"

namespace demo {

inline Vector3 add(Vector3 a, Vector3 b) { return Vector3{a.x + b.x, a.y + b.y, a.z + b.z}; }
inline Vector3 scale(Vector3 v, float s) { return Vector3{v.x * s, v.y * s, v.z * s}; }
inline float length(Vector3 v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

inline shimera::Vec3<float> toVec3(Vector3 v) { return {v.x, v.y, v.z}; }

}

#endif //SHIMERA_DEMO_MATHUTILS_HPP
