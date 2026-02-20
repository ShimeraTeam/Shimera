#ifndef SHIMERA_IPOSTPROCCESSOR_HPP
#define SHIMERA_IPOSTPROCCESSOR_HPP

#include "IShader.hpp"
#include "ITexture.hpp"
#include "uniform/Uniform.hpp"

class IPostProccessor {
    public:
        virtual ~IPostProccessor() = default;

        virtual void setShader(const std::string& vert, const std::string& frag) = 0;
        virtual void render(ITexture& texture) = 0;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
        virtual IShader& getShader() = 0;
        virtual void bindShader() = 0;
};

#endif //SHIMERA_IPOSTPROCCESSOR_HPP