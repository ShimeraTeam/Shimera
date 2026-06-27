#ifndef SHIMERA_SFMLPOSTPROCESSOR_HPP
#define SHIMERA_SFMLPOSTPROCESSOR_HPP

#include "backend/IPostProcessor.hpp"
#include "backend/IShader.hpp"
#include <vector>
#include <memory>

namespace shimera {

class SFMLPostProcessor final : public IPostProcessor {
    public:
        SFMLPostProcessor();
        ~SFMLPostProcessor() override;

        // Prevent copying
        SFMLPostProcessor(const SFMLPostProcessor&) = delete;
        SFMLPostProcessor& operator=(const SFMLPostProcessor&) = delete;

        void setShader(const std::string& vert, const std::string& frag) override;
        void render(ITexture& texture) override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        void addInputTexture(const std::string& uniformName, ITexture& texture, unsigned int unit) override;
        IShader& getShader() override;
        void bindShader() override;

    private:
        std::vector<ExtraTex> m_extraTextures;

        void initializeQuad();
        void cleanupQuad();
        void cleanupShader();

        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;

        std::unique_ptr<IShader> m_shader;
};

}

#endif //SHIMERA_SFMLPOSTPROCESSOR_HPP
