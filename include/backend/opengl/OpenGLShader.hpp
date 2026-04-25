#ifndef SHIMERA_OPENGL_OPENGLSHADER_HPP
#define SHIMERA_OPENGL_OPENGLSHADER_HPP

#include <string>
#include <unordered_map>

#include "backend/IShader.hpp"

namespace shimera {

/**
 * OpenGL Shader implementation using raw OpenGL shader management
 */
class OpenGLShader final : public IShader {
    public:
        OpenGLShader();
        ~OpenGLShader() override;

        OpenGLShader(const OpenGLShader&) = delete;
        OpenGLShader& operator=(const OpenGLShader&) = delete;

        void bind() const override;
        void unbind() const override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        [[nodiscard]] uint32_t getNativeHandle() const override;

        void loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    private:
        int getUniformLocation(const std::string& name);

        uint32_t programId;
        std::unordered_map<std::string, int> uniformCache;
};

}

#endif //SHIMERA_OPENGL_OPENGLSHADER_HPP
