#ifndef SHIMERA_RAYLIBPOSTPROCESSOR_HPP
#define SHIMERA_RAYLIBPOSTPROCESSOR_HPP

#include "backend/IPostProccessor.hpp"
#include "backend/IShader.hpp"
#include <memory>

class RaylibPostProcessor final : public IPostProccessor {
    public:
        RaylibPostProcessor();
        ~RaylibPostProcessor() override;

        // Prevent copying
        RaylibPostProcessor(const RaylibPostProcessor&) = delete;
        RaylibPostProcessor& operator=(const RaylibPostProcessor&) = delete;

        void setShader(const std::string& vert, const std::string& frag) override;
        void render(ITexture& texture) override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        IShader& getShader() override;
        void bindShader() override;

    private:
        void initializeQuad();
        void cleanupQuad();
        void cleanupShader();

        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;

        std::unique_ptr<IShader> shader;
};

#endif //SHIMERA_RAYLIBPOSTPROCESSOR_HPP