#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "uniform/UniformBlock.hpp"
#include "uniform/UniformValue.inl"

namespace shimera {

/**
 * A linked GL program plus a uniform-location cache.
 *
 * (Same as beta's OpenGLShader, but with real shader info and two separate load methods)
 */
class GLShader {
    public:
        // Both throw std::runtime_error carrying the shader name and the GL info log.
        static std::unique_ptr<GLShader> fromFiles(const std::string& vertPath, const std::string& fragPath);
        static std::unique_ptr<GLShader> fromMemory(std::string_view vertSource, std::string_view fragSource,
                                                    std::string name = "<memory>");

        ~GLShader();

        GLShader(const GLShader&) = delete;
        GLShader& operator=(const GLShader&) = delete;
        GLShader(GLShader&&) = delete;
        GLShader& operator=(GLShader&&) = delete;

        void bind();

        // Binds the program itself, a uniform cannot be set on an unbound program.
        void setUniform(const std::string& name, const UniformValue& value);

        [[nodiscard]] uint32_t handle() const { return m_program; }
        [[nodiscard]] const std::string& name() const { return m_name; }

    private:
        GLShader(std::string_view vertSource, std::string_view fragSource, std::string name);

        int uniformLocation(const std::string& uniform);

        static uint32_t compile(uint32_t stage, std::string_view source, const std::string& name);

        uint32_t m_program = 0;
        std::string m_name;
        std::unordered_map<std::string, int> m_uniformCache;
        std::vector<UniformBlock> m_blocks;
};

}
