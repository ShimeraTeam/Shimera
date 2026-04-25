#include "backend/opengl/OpenGLShader.hpp"

#include <GL/glew.h>
#include <glUtils.h>

#include <iostream>
#include <stdexcept>
#include <variant>

#include "uniform/Vec3.hpp"
#include "uniform/Vec4.hpp"

using shimera::OpenGLShader;

OpenGLShader::OpenGLShader() : m_programId(0) {}

OpenGLShader::~OpenGLShader() {
    if (m_programId != 0) {
        GLC(glDeleteProgram(m_programId));
    }
}

void OpenGLShader::loadFromFiles(const std::string& vertPath, const std::string& fragPath) {
    ShaderProgramSource source = parseShader(vertPath, fragPath);

    m_programId = createShader(source.vertex, source.fragment);

    if (m_programId == 0) {
        throw std::runtime_error("Failed to create shader program from files: " + vertPath + ", " + fragPath);
    }

    bind();
    int location = getUniformLocation("u_screenTexture");
    if (location != -1) {
        GLC(glUniform1i(location, 0));
    }
    unbind();
}

void OpenGLShader::bind() const {
    GLC(glUseProgram(m_programId));
}

void OpenGLShader::unbind() const {
    GLC(glUseProgram(0));
}

void OpenGLShader::setUniform(const std::string& name, const UniformValue& value) {
    const int location = getUniformLocation(name);
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader." << std::endl;
        return;
    }

    bind();

    std::visit([location](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, float>) {
            GLC(glUniform1f(location, val));
        } else if constexpr (std::is_same_v<T, int>) {
            GLC(glUniform1i(location, val));
        } else if constexpr (std::is_same_v<T, Vec2<float>>) {
            GLC(glUniform2f(location, val.x, val.y));
        } else if constexpr (std::is_same_v<T, Vec3<float>>) {
            GLC(glUniform3f(location, val.x, val.y, val.z));
        } else if constexpr (std::is_same_v<T, Vec4<float>>) {
            GLC(glUniform4f(location, val.x, val.y, val.z, val.w));
        }
    }, value);
}

uint32_t OpenGLShader::getNativeHandle() const {
    return m_programId;
}

int OpenGLShader::getUniformLocation(const std::string& name) {
    const auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }

    GLC(int location = glGetUniformLocation(m_programId, name.c_str()));
    m_uniformCache[name] = location;

    return location;
}
