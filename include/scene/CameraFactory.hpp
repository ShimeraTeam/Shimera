#ifndef TEST_MEMORY_SFML_CAMERAFACTORY_HPP
#define TEST_MEMORY_SFML_CAMERAFACTORY_HPP

#include "Camera.hpp"
#include "shimera_api.h"

namespace shimera {

class SHIMERA_API CameraFactory {
    public:
        static Camera perspective(const Vec3<float>& position, const Vec3<float>& target, const Vec3<float>& up,
                                  float fovYDegrees, float aspect, float nearPlane, float farPlane);
};

}

#endif //TEST_MEMORY_SFML_CAMERAFACTORY_HPP
