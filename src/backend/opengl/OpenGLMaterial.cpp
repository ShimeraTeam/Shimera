#include "backend/opengl/OpenGLMaterial.hpp"


shimera::OpenGLMaterial::OpenGLMaterial(const std::string& vert, const std::string& frag) {
    m_shader.loadFromFiles(vert, frag);
}

void shimera::OpenGLMaterial::setUniform(const std::string& name, const UniformValue& value) {
    m_shader.setUniform(name, value);
}
