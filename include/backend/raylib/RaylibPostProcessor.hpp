#ifndef SHIMERA_RAYLIBPOSTPROCESSOR_HPP
#define SHIMERA_RAYLIBPOSTPROCESSOR_HPP

#include "backend/IPostProcessor.hpp"
#include "backend/IShader.hpp"
#include <memory>
#include <vector>

namespace shimera {

class RaylibPostProcessor final : public IPostProcessor {
    public:
        RaylibPostProcessor();
        ~RaylibPostProcessor() override;

        // Prevent copying
        RaylibPostProcessor(const RaylibPostProcessor&) = delete;
        RaylibPostProcessor& operator=(const RaylibPostProcessor&) = delete;

        void setShader(const std::string& vert, const std::string& frag) override;
        void render(ITexture& texture) override;
        void addInputTexture(const std::string& uniformName, ITexture& texture, unsigned int unit) override;
        void setUniform(const std::string& name, const UniformValue& value) override;
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

#endif //SHIMERA_RAYLIBPOSTPROCESSOR_HPP