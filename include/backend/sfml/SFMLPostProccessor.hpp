#ifndef SHIMERA_SFML_SFMLPOSTPROCCESSOR_HPP
#define SHIMERA_SFML_SFMLPOSTPROCCESSOR_HPP

#include "backend/IPostProccessor.hpp"
#include "backend/IShader.hpp"
#include <memory>

class SFMLPostProccessor final : public IPostProccessor {
    public:
        SFMLPostProccessor();
        ~SFMLPostProccessor() override;

        // Prevent copying
        SFMLPostProccessor(const SFMLPostProccessor&) = delete;
        SFMLPostProccessor& operator=(const SFMLPostProccessor&) = delete;

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

#endif //SHIMERA_SFML_SFMLPOSTPROCCESSOR_HPP