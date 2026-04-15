#ifndef SHIMERA_ISHADER_HPP
#define SHIMERA_ISHADER_HPP

#include <shimera_api.h>
#include <cstdint>
#include <string>

#include "uniform/Uniform.hpp"

class SHIMERA_API IShader {
    public:
        virtual ~IShader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
        [[nodiscard]] virtual uint32_t getNativeHandle() const = 0;
};

#endif //SHIMERA_ISHADER_HPP