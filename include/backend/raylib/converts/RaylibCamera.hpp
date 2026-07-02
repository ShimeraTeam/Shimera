#ifndef SHIMERA_RAYLIBCAMERA_HPP
#define SHIMERA_RAYLIBCAMERA_HPP

#include <raylib.h>

#include "shimera_api.h"
#include "scene/Camera.hpp"

namespace shimera {

SHIMERA_API class RaylibCamera {
    public:
        static Camera toShimera(const Camera3D& cam, float nearPlane = 0.01f,
                                float farPlane = 1000.0f, float aspect = -1.0f);
};

}
#endif //SHIMERA_RAYLIBCAMERA_HPP
