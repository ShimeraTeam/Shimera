#pragma once

#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

/**
 * A target independent world position information.
 */
struct Transform {
    Vec3<float> position{0.0f, 0.0f, 0.0f};
    Vec3<float> rotationEuler{0.0f, 0.0f, 0.0f}; // degrees, applied Z then Y then X
    Vec3<float> scale{1.0f, 1.0f, 1.0f};

    [[nodiscard]] Mat4 toMatrix() const;
};

}
