#ifndef SHIMERA_TRANSFORMFACTORY_HPP
#define SHIMERA_TRANSFORMFACTORY_HPP

#include "shimera_api.h"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

class SHIMERA_API TransformFactory {
    public:
        static Mat4 model(const Vec3<float>& position,
            const Vec3<float>& rotationEuler = Vec3(0.0f, 0.0f, 0.0f),
            const Vec3<float>& scale = Vec3(1.0f, 1.0f, 1.0f));
};

}

#endif //SHIMERA_TRANSFORMFACTORY_HPP
