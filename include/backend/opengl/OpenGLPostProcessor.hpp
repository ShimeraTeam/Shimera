#ifndef SHIMERA_OPENGL_OPENGLPOSTPROCESSOR_HPP
#define SHIMERA_OPENGL_OPENGLPOSTPROCESSOR_HPP

#include <memory>

#include "backend/IPostProcessor.hpp"

namespace shimera {

/**
 * OpenGL Post-Processor implementation
 * Renders a full-screen quad with a post-processing shader and input texture
 */
class OpenGLPostProcessor final : public IPostProcessor {
    public:
        OpenGLPostProcessor();
        ~OpenGLPostProcessor() override;

        OpenGLPostProcessor(const OpenGLPostProcessor&) = delete;
        OpenGLPostProcessor& operator=(const OpenGLPostProcessor&) = delete;

        void setShader(const std::string& vert, const std::string& frag) override;
        void render(ITexture& texture) override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        IShader& getShader() override;
        void bindShader() override;

    private:
        void initializeQuad();
        void cleanupQuad();
        void cleanupShader();

        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;

        std::unique_ptr<IShader> m_shader;
};

}

#endif //SHIMERA_OPENGL_OPENGLPOSTPROCESSOR_HPP
