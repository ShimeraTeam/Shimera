#ifndef TEST_MEMORY_SFML_IMATERIAL_HPP
#define TEST_MEMORY_SFML_IMATERIAL_HPP

#include "shimera_api.h"
#include "uniform/Uniform.inl"

namespace shimera {

class SHIMERA_API IMaterial {
    public:
        virtual ~IMaterial() = default;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
};

}

#endif //TEST_MEMORY_SFML_IMATERIAL_HPP
