#ifndef SHIMERA_CAMERA_HPP
#define SHIMERA_CAMERA_HPP

#include "shimera_api.h"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

struct SHIMERA_API Camera {
    Mat4 view;
    Mat4 projection;
    Vec3<float> position {0.0f, 0.0f, 0.0f};
};

}

#endif //SHIMERA_CAMERA_HPP
