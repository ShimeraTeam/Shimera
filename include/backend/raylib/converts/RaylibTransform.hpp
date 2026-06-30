#ifndef TEST_MEMORY_SFML_RAYLIBTRANSFORM_HPP
#define TEST_MEMORY_SFML_RAYLIBTRANSFORM_HPP

#include "shimera_api.h"

#include <raylib.h>

#include "uniform/Mat4.hpp"

namespace shimera {

class SHIMERA_API RaylibTransform {
    public:
        static Mat4 toModel(Vector3 position, Vector3 rotationAxis  = {0.0f, 1.0f, 0.0f},
            float rotationAngle = 0.0f, Vector3 scale = {1.0f, 1.0f, 1.0f});
};

}

#endif //TEST_MEMORY_SFML_RAYLIBTRANSFORM_HPP
