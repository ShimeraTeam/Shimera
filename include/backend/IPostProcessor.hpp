#ifndef SHIMERA_IPOSTPROCESSOR_HPP
#define SHIMERA_IPOSTPROCESSOR_HPP

#include <shimera_api.h>
#include "IShader.hpp"
#include "ITexture.hpp"
#include "uniform/Uniform.inl"

namespace shimera {

class SHIMERA_API IPostProcessor {
    public:
        virtual ~IPostProcessor() = default;

        virtual void setShader(const std::string& vert, const std::string& frag) = 0;
        virtual void render(ITexture& texture) = 0;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
        virtual IShader& getShader() = 0;
        virtual void bindShader() = 0;
};

}

#endif //SHIMERA_IPOSTPROCESSOR_HPP