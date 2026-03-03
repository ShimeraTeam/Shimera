#ifndef SHIMERA_RAYLIBSHADER_HPP
#define SHIMERA_RAYLIBSHADER_HPP

#include "backend/IShader.hpp"
#include <string>
#include <unordered_map>

/**
 * Raylib Shader implementation using OpenGL
 * Since Raylib uses OpenGL underneath, we use raw OpenGL shader management
 */
//TODO: Maybe later try to share the same code between SFML, OpenGL and Raylib backends, maybe...
class RaylibShader final : public IShader {
    public:
        RaylibShader();
        ~RaylibShader() override;

        // Prevent copying
        RaylibShader(const RaylibShader&) = delete;
        RaylibShader& operator=(const RaylibShader&) = delete;

        void bind() const override;
        void unbind() const override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        uint32_t getNativeHandle() const override;

        void loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    private:
        int getUniformLocation(const std::string& name);
    
        uint32_t programId;
        // Uniform cache to lower the CPU and GPU communications
        std::unordered_map<std::string, int> uniformCache;
};

#endif //SHIMERA_RAYLIBSHADER_HPP
