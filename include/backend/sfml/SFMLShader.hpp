#ifndef SHIMERA_SFML_SFMLSHADER_HPP
#define SHIMERA_SFML_SFMLSHADER_HPP

#include "backend/IShader.hpp"
#include <string>
#include <unordered_map>

namespace shimera {

/**
 * SFML Shader implementation using OpenGL
 * Since SFML uses OpenGL underneath, we use raw OpenGL shader management
 */
//TODO: Maybe later try to share the same code between SFML and OpenGL backends, maybe...
class SFMLShader final : public IShader {
    public:
        SFMLShader();
        ~SFMLShader() override;

        // Prevent copying
        SFMLShader(const SFMLShader&) = delete;
        SFMLShader& operator=(const SFMLShader&) = delete;

        void bind() const override;
        void unbind() const override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        uint32_t getNativeHandle() const override;

        void loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    private:
        int getUniformLocation(const std::string& name);
    
        uint32_t m_programId;
        // Uniform cache to lower the CPU and GPU communications
        std::unordered_map<std::string, int> m_uniformCache;
};

}

#endif //SHIMERA_SFML_SFMLSHADER_HPP
