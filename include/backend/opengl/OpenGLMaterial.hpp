#ifndef SHIMERA_OPENGLMATERIAL_HPP
#define SHIMERA_OPENGLMATERIAL_HPP

#include "shimera_api.h"
#include "OpenGLShader.hpp"
#include "backend/IMaterial.hpp"

namespace shimera {

SHIMERA_API class OpenGLMaterial : public IMaterial {
    public:
        OpenGLMaterial(const std::string& vert, const std::string& frag);
        void setUniform(const std::string& name, const UniformValue& value) override;
        OpenGLShader& shader() { return m_shader; }
    private:
        OpenGLShader m_shader;
};

}

#endif //SHIMERA_OPENGLMATERIAL_HPP
