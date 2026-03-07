#include "backend/raylib/RaylibShader.hpp"
#include <GL/glew.h>
#include <glUtils.h>
#include <iostream>
#include <stdexcept>
#include <variant>
#include "uniform/Vec4.hpp"

RaylibShader::RaylibShader() : programId(0) {}

RaylibShader::~RaylibShader() {
    if (programId != 0) {
        GLC(glDeleteProgram(programId));
    }
}

void RaylibShader::loadFromFiles(const std::string& vertPath, const std::string& fragPath) {
    ShaderProgramSource source = parseShader(vertPath, fragPath);
    
    programId = createShader(source.vertex, source.fragment);
    
    if (programId == 0) {
        throw std::runtime_error("Failed to create shader program from files: " + vertPath + ", " + fragPath);
    }
    
    // Set up the texture sampler uniform (u_screenTexture) which is automatically bound to texture unit 0
    bind();
    int location = getUniformLocation("u_screenTexture");
    if (location != -1) {
        GLC(glUniform1i(location, 0));
    }
    unbind();
}

void RaylibShader::bind() const {
    GLC(glUseProgram(programId));
}

void RaylibShader::unbind() const {
    GLC(glUseProgram(0));
}

void RaylibShader::setUniform(const std::string& name, const UniformValue& value) {
    int location = getUniformLocation(name);
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
        } else if constexpr (std::is_same_v<T, Vec4<float>>) {
            GLC(glUniform4f(location, val.x, val.y, val.z, val.w));
        }
    }, value);
}

uint32_t RaylibShader::getNativeHandle() const {
    return programId;
}

int RaylibShader::getUniformLocation(const std::string& name) {
    // Cache checking
    auto it = uniformCache.find(name);
    if (it != uniformCache.end()) {
        return it->second;
    }

    GLC(int location = glGetUniformLocation(programId, name.c_str()));
    uniformCache[name] = location;
    
    return location;
}
